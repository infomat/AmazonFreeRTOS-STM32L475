#include <stdint.h>
#include <stdarg.h>
#include <memory.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_hsensor.h"

#include "aws_mqtt_agent.h"

#include "aws_clientcredential.h"
#include "aws_system_init.h"
#include "aws_logging_task.h"

/**
 * @brief MQTT client ID.
 *
 * It must be unique per MQTT broker.
 */
#define mqttCLIENT_ID            ( ( const uint8_t * ) "STM32L475" )

/**
 * @brief The topic that the MQTT client both subscribes and publishes to.
 */
#define mqttTOPIC_NAME           ( ( const uint8_t * ) "freertos/mqtt/basement" )

/**
 * @brief Dimension of the character array buffers used to hold data (strings in
 * this case) that is published to and received from the MQTT broker (in the cloud).
 */
#define mqttMAX_DATA_LENGTH      20

/* Timeout used when performing MQTT operations that do not need extra time
 * to perform a TLS negotiation. */
#define democonfigMQTT_TIMEOUT  pdMS_TO_TICKS( 2500 )

/* Timeout used when establishing a connection, which required TLS
 * negotiation. */
#define democonfigMQTT_Connect_TIMEOUT          pdMS_TO_TICKS( 12000 )
/**
 * @ brief The handle of the MQTT client object used by the MQTT echo demo.
 */
static MQTTAgentHandle_t xMQTTHandle = NULL;

/**
 * @ brief vMQTTDemoTask Task Handler
 */
TaskHandle_t mqttTaskHandle = NULL;

/**
 * @brief Creates an MQTT client and then connects to the MQTT broker.
 *
 * The MQTT broker end point is set by clientcredentialMQTT_BROKER_ENDPOINT.
 *
 * @return pdPASS if everything is successful, pdFAIL otherwise.
 */
static BaseType_t prvCreateClientAndConnectToBroker( void );

/**
 * @brief Publishes the next message to the echoTOPIC_NAME topic.
 *
 * This is called every five seconds to publish the next message.
 *
 */
static void prvPublishNextMessage( void );


static BaseType_t prvCreateClientAndConnectToBroker( void )
{
    MQTTAgentReturnCode_t xReturned;
    BaseType_t xReturn = pdFAIL;
    MQTTAgentConnectParams_t xConnectParameters =
    {
        clientcredentialMQTT_BROKER_ENDPOINT, /* The URL of the MQTT broker to connect to. */
        0,   /* Connection flags. */
        pdFALSE,                              /* Deprecated. */
        clientcredentialMQTT_BROKER_PORT,     /* Port number on which the MQTT broker is listening. Can be overridden by ALPN connection flag. */
        mqttCLIENT_ID,                        /* Client Identifier of the MQTT client. It should be unique per broker. */
        0,                                    /* The length of the client Id, filled in later as not const. */
        pdFALSE,                              /* Deprecated. */
        NULL,                                 /* User data supplied to the callback. Can be NULL. */
        NULL,                                 /* Callback used to report various events. Can be NULL. */
        NULL,                                 /* Certificate used for secure connection. Can be NULL. */
        0                                     /* Size of certificate used for secure connection. */
    };

    /* Check this function has not already been executed. */
    configASSERT( xMQTTHandle == NULL );

    /* The MQTT client object must be created before it can be used.  The
     * maximum number of MQTT client objects that can exist simultaneously
     * is set by mqttconfigMAX_BROKERS. */
    xReturned = MQTT_AGENT_Create( &xMQTTHandle );

    if( xReturned == eMQTTAgentSuccess )
    {
        /* Fill in the MQTTAgentConnectParams_t member that is not const,
         * and therefore could not be set in the initializer (where
         * xConnectParameters is declared in this function). */
        xConnectParameters.usClientIdLength = ( uint16_t ) strlen( ( const char * ) mqttCLIENT_ID );

        /* Connect to the broker. */
        configPRINTF( ( "MQTT client attempting to connect to %s.\r\n", clientcredentialMQTT_BROKER_ENDPOINT ) );
        xReturned = MQTT_AGENT_Connect( xMQTTHandle,
                                        &xConnectParameters,
                                        democonfigMQTT_Connect_TIMEOUT );

        if( xReturned != eMQTTAgentSuccess )
        {
            /* Could not connect, so delete the MQTT client. */
            ( void ) MQTT_AGENT_Delete( xMQTTHandle );
            configPRINTF( ( "ERROR:  MQTT failed to connect with error %d.\r\n", xReturned ) );
        }
        else
        {
            configPRINTF( ( "MQTT connected.\r\n" ) );
            xReturn = pdPASS;
        }
    }

    return xReturn;
}
/*-----------------------------------------------------------*/

static void prvPublishNextMessage( void )
{
    MQTTAgentPublishParams_t xPublishParameters;
    MQTTAgentReturnCode_t xReturned;

    /* Check this function is not being called before the MQTT client object has
     * been created. */
    configASSERT( xMQTTHandle != NULL );

    /* Create the message that will be published as json which has temperature and humidity
     * where n is a monotonically increasing number. Note that snprintf appends
     * terminating null character to the cDataBuffer. */
    char cDataBuffer[ mqttMAX_DATA_LENGTH ];

    float temp = BSP_TSENSOR_ReadTemp();
    vTaskDelay( pdMS_TO_TICKS( 1000UL ) );
    float humidity = BSP_HSENSOR_ReadHumidity();
    vTaskDelay( pdMS_TO_TICKS( 1000UL ) );

    snprintf(cDataBuffer, mqttMAX_DATA_LENGTH, "{T: %d, H: %d}", (int)temp, (int)humidity);

    /* Setup the publish parameters. */
    memset( &( xPublishParameters ), 0x00, sizeof( xPublishParameters ) );
    xPublishParameters.pucTopic = mqttTOPIC_NAME;
    xPublishParameters.pvData = cDataBuffer;
    xPublishParameters.usTopicLength = ( uint16_t ) strlen( ( const char * ) mqttTOPIC_NAME );
    xPublishParameters.ulDataLength = ( uint32_t ) strlen( cDataBuffer );
    xPublishParameters.xQoS = eMQTTQoS1;

    /* Publish the message. */
    xReturned = MQTT_AGENT_Publish( xMQTTHandle,
                                    &( xPublishParameters ),
                                    democonfigMQTT_TIMEOUT );

    if( xReturned == eMQTTAgentSuccess )
    {
        configPRINTF( ( "Successfully published '%s'\r\n", cDataBuffer ) );
    }
    else
    {
        configPRINTF( ( "ERROR:  Failed to publish '%s'\r\n", cDataBuffer ) );
    }

    /* Remove compiler warnings in case configPRINTF() is not defined. */
    ( void ) xReturned;
}



/*-----------------------------------------------------------*/

static void vMQTTDemoTask( void* pvParameters ) {
    BaseType_t xReturned;


     /* Create the MQTT client object and connect it to the MQTT broker. */
    xReturned = prvCreateClientAndConnectToBroker();

   if( xReturned == pdPASS )
    {
        while (pdTRUE) {
           
            prvPublishNextMessage();

            vTaskDelay( pdMS_TO_TICKS( 5000UL ) );
        }
    }    
    else
    {
        configPRINTF( ( "MQTT client could not connect to broker.\r\n" ) );
    }

    MQTT_AGENT_Disconnect(xMQTTHandle, pdMS_TO_TICKS( 2500 ) );

    configPRINTF(("Done sending messages\n"));
    vTaskDelete(NULL);
}

/*-----------------------------------------------------------*/

void vStartMQTTDemo()
{
	configPRINTF( ( "Creating MQTT Demo Task...\r\n" ) );
	BSP_TSENSOR_Init();
	BSP_HSENSOR_Init();
	xTaskCreate(vMQTTDemoTask, 
				"mqttDemoTask", 
				configMINIMAL_STACK_SIZE * 3, 
				NULL, 
				tskIDLE_PRIORITY, 
				&mqttTaskHandle);
}
