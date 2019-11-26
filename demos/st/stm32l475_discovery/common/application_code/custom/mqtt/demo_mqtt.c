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

#define mqttMAX_DATA_LENGTH      20

static MQTTAgentHandle_t xMQTTHandle = NULL;
static const uint8_t *mqttCLIENT_ID = "STM32L475";
static const uint8_t *mqttTOPIC_NAME = "freertos/mqtt/basement";
TaskHandle_t mqttTaskHandle = NULL;
MQTTAgentReturnCode_t retCode = eMQTTAgentSuccess;

static MQTTBool_t prvMQTTCallback( void * pvUserData,
                                   const MQTTPublishData_t * const pxPublishParameters );

static void vMQTTDemoTask( void* pvParameters ) {
    MQTTAgentConnectParams_t xConnectParameters = {
        clientcredentialMQTT_BROKER_ENDPOINT,
		NULL,
        pdFALSE,
        clientcredentialMQTT_BROKER_PORT,
		mqttCLIENT_ID,
		( uint16_t ) strlen( ( const char * ) mqttCLIENT_ID ),
		pdFALSE,
        NULL,
        NULL,
        NULL,
        0
    };

    retCode = MQTT_AGENT_Create( &xMQTTHandle );
	configPRINTF(("MQTT_AGENT_Create: %d\n", retCode));

	retCode = MQTT_AGENT_Connect(xMQTTHandle, &xConnectParameters, pdMS_TO_TICKS(12000));
	configPRINTF(("MQTT_AGENT_Connect: %d\n", retCode));

//	MQTTAgentSubscribeParams_t xSubscribeParams;
//	xSubscribeParams.pucTopic = mqttTOPIC_NAME;
//	xSubscribeParams.pvPublishCallbackContext = NULL;
//	xSubscribeParams.pxPublishCallback = prvMQTTCallback;
//	xSubscribeParams.usTopicLength = ( uint16_t ) strlen( ( const char * ) mqttTOPIC_NAME );
//	xSubscribeParams.xQoS = eMQTTQoS1;
//
//	MQTT_AGENT_Subscribe( xMQTTHandle,
//	 				      &xSubscribeParams,
//						  pdMS_TO_TICKS( 2500 ));



	while (pdTRUE) {
	    char cDataBuffer[ mqttMAX_DATA_LENGTH ];

	    float temp = BSP_TSENSOR_ReadTemp();
	    vTaskDelay( pdMS_TO_TICKS( 1000UL ) );
		float humidity = BSP_HSENSOR_ReadHumidity();
		vTaskDelay( pdMS_TO_TICKS( 1000UL ) );

	    snprintf(cDataBuffer, mqttMAX_DATA_LENGTH, "{T: %d, H: %d}", (int)temp, (int)humidity);

	    MQTTAgentPublishParams_t xPublishParameters;
	    memset( &( xPublishParameters ), 0x00, sizeof( xPublishParameters ) );
	    xPublishParameters.pucTopic = mqttTOPIC_NAME;
	    xPublishParameters.pvData = cDataBuffer;
	    xPublishParameters.usTopicLength = ( uint16_t ) strlen( ( const char * ) mqttTOPIC_NAME );
	    xPublishParameters.ulDataLength = ( uint32_t ) strlen( cDataBuffer );
	    xPublishParameters.xQoS = eMQTTQoS1;

	    MQTT_AGENT_Publish( xMQTTHandle,
                            &( xPublishParameters ),
							pdMS_TO_TICKS( 2500 ));

	    configPRINTF(("Sent message: %s\n", cDataBuffer));

	    vTaskDelay( pdMS_TO_TICKS( 5000UL ) );
	}

    MQTT_AGENT_Disconnect(xMQTTHandle, pdMS_TO_TICKS( 2500 ) );

    configPRINTF(("Done sending messages\n"));
    vTaskDelete(NULL);
}

static MQTTBool_t prvMQTTCallback( void * pvUserData,
                                   const MQTTPublishData_t * const pxPublishParameters )
{
    char cBuffer[ mqttMAX_DATA_LENGTH];
	uint32_t ulBytesToCopy = pxPublishParameters->ulDataLength;

	memset( cBuffer, 0x00, sizeof( cBuffer ) );
	memcpy( cBuffer, pxPublishParameters->pvData, ( size_t ) ulBytesToCopy );

	configPRINTF(("Received message: %s\n", cBuffer));

    return eMQTTFalse;
}

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
