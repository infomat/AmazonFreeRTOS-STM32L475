/*
 * Amazon FreeRTOS V1.4.8
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

#ifndef AWS_CLIENT_CREDENTIAL_KEYS_H
#define AWS_CLIENT_CREDENTIAL_KEYS_H

#include <stdint.h>

/*
 * PEM-encoded client certificate
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 */
#define keyCLIENT_CERTIFICATE_PEM \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDWTCCAkGgAwIBAgIUdl8/Mlb/thLJYvzBzu5Bb3hBHnEwDQYJKoZIhvcNAQEL\n"\
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"\
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE5MDYwMjE5NDYy\n"\
"NVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"\
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALUmmQcSqI7S8DOWE7HW\n"\
"jJnGnApWpv2HmcchBlCcGPuez+2IhULb30h6lFR14oA5v+Ev2rOsXPv7fx3CGEDy\n"\
"Z+gu8YNwxFd5Cn1zfyRfUroEMy+L0fxrWCmfkRgyQmdRa2xgDWAPwPasxEGDZ0tI\n"\
"RJP7NjphjKqfs+CLzx9BW1VvYI0T9LQzEnij43dd5WJGWyNKOjKJqrAvrRAOmfq/\n"\
"8LCLWH1R9vfugCbbFPc5BHvbHJ1v/+6T6uVDmQTK6oTzexJa3o5yvOjzlZ/k2PJX\n"\
"NiYYa5BOCEkXUFDLqqQwdnzgWcnWNfq68M4v0ye+jQ8MTBfTFgqKUJH9Ty9Aph1Q\n"\
"BhUCAwEAAaNgMF4wHwYDVR0jBBgwFoAU2rp6wVqYkDioX4I0Km9pCy9nrakwHQYD\n"\
"VR0OBBYEFM0pkMgu6PJMAxvuD+IUJ2Hw66rqMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"\
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCGja1AO52iWifk3+SbqyBv+v05\n"\
"bbKs1tvlcBT3i7WWKiWYK+khe8AttXLIc+W+w2F/Fpgs+eFg2qFVgBZahegSnfA1\n"\
"QA2hLFxNE8cmhKqpcsLaqPaV/ubPN8dfIfABqDWPDr7IHwQWoFs85rGCpNw7rbm3\n"\
"Ufx1tD0UgtIAbZEUySKm4Hwau2aZxUvACyJ3lJlFgAWiwi+Jh6ouCgPYNtAdemvo\n"\
"HQklT5PXcgrf+vON5TIdilyiZA6SWaiySVrel+zSxfO3HPq/6O1WE3xIgaQGEsBH\n"\
"L0igk64Yq+TAtkP+M5PjlLcEV5UnpGMCLDAPXMyBc7t+r5OTCShzmXhm1cEs\n"\
"-----END CERTIFICATE-----\n"

/*
 * PEM-encoded issuer certificate for AWS IoT Just In Time Registration (JITR).
 * This is required if you're using JITR, since the issuer (Certificate 
 * Authority) of the client certificate is used by the server for routing the 
 * device's initial request. (The device client certificate must always be 
 * sent as well.) For more information about JITR, see:
 *  https://docs.aws.amazon.com/iot/latest/developerguide/jit-provisioning.html, 
 *  https://aws.amazon.com/blogs/iot/just-in-time-registration-of-device-certificates-on-aws-iot/.
 *
 * If you're not using JITR, set below to NULL.
 * 
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 */
#define keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM  NULL

/*
 * PEM-encoded client private key.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN RSA PRIVATE KEY-----\n"\
 * "...base64 data...\n"\
 * "-----END RSA PRIVATE KEY-----\n"
 */
#define keyCLIENT_PRIVATE_KEY_PEM \
"-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEowIBAAKCAQEAtSaZBxKojtLwM5YTsdaMmcacClam/YeZxyEGUJwY+57P7YiF\n"\
"QtvfSHqUVHXigDm/4S/as6xc+/t/HcIYQPJn6C7xg3DEV3kKfXN/JF9SugQzL4vR\n"\
"/GtYKZ+RGDJCZ1FrbGANYA/A9qzEQYNnS0hEk/s2OmGMqp+z4IvPH0FbVW9gjRP0\n"\
"tDMSeKPjd13lYkZbI0o6MomqsC+tEA6Z+r/wsItYfVH29+6AJtsU9zkEe9scnW//\n"\
"7pPq5UOZBMrqhPN7ElrejnK86POVn+TY8lc2JhhrkE4ISRdQUMuqpDB2fOBZydY1\n"\
"+rrwzi/TJ76NDwxMF9MWCopQkf1PL0CmHVAGFQIDAQABAoIBADalcCBcFpEPPPqy\n"\
"2ToamMuCyGut/C4bC96wLbF/hWHSZLMpLdofbdn1cG2SKkO4KBQMVZNB+cLh+VTG\n"\
"yn49Ie8+ygh2qiQhFXMGtmIIh3Ip9i7vpObakvvrcN+1uaZlP1ywFRjXZ8SCFaoV\n"\
"s8FYCagCbjdqWf7WsnKYemDZXG4G608D3osXNCb3CvJGRp5mjCtCO1jh6w8Cxyxk\n"\
"eISjJmuUxqHIDRM1UxajQDID6b5/c0A/kaVAyKFXM1lhATyM+POlS6miSZ25S19K\n"\
"QwRiC2xwTIrmOeblRyN8nmPks83aLtiU5EreHbU08F/fsyK83bcxZ8Q5IV3u88xb\n"\
"EGAAh3kCgYEA2mJo3gthaXPs7As6/L/bUBZ8h4LncCna35gVzt+kySYfCYoVS3Dq\n"\
"KBIx1wJoqVpguiZkNs5AWLsjUS5kpoQyCj+Z8W4Kk8sVRgPtXNc611ZYYvPtzb2q\n"\
"xg2XTHFWuxvOFceKtOYVESE5Sw6f9ttxnlybnt6OulJxQ7+gohTAVe8CgYEA1Fph\n"\
"/BNq7vC7ZWtX9jjX76cSWGROcy1ZM+5hnbFRWEv+a9GqIQkSF4s3ii7gGt95lcXD\n"\
"yoOc94brkdHwwl076wBOACzj6P5WxgGsF50ujZpGU7mUJLlgmcqTXN8H8AW0OLvH\n"\
"T3aqeeFLTPZ+PX7w4vn6fVcd7sd0Z0dXwFk/SDsCgYEAnA08YIcDWeaDw3yW7ytt\n"\
"3UHkRLVPCoBuLd6wnr6DM1WxUhDA/MK4IGA4CytmfLXfbsWmUN97bax0JTugorjO\n"\
"RTKDn9kBZ/Dzx69guzn1ixDhYfxqzt0cnh/yIKbQ0i2hS0DmpjPPlAkIttDBkBXp\n"\
"j78fWRwyI4Rr+XyCna90bTMCgYB6FZRrxgYeZQx33GTvy/Jpd5YWd2Sa1yGHt68r\n"\
"Z/wqVVZqSloTMBHUs1c0/IofKQfP0akkztH5hPn+FcLjG5MNIlm1xBkrpL3ijKeR\n"\
"0+4lvuJb1D+VdXZSNKLQK/F2TvE0BNEroD+WXq5aCwS8+jQnaixWzzUISGdl0SBk\n"\
"F2tIbQKBgFvMH6Er4WHQW0aE5dhUM/tsMpB9g5fBLle/xGJa/WLHish894+c9Bwc\n"\
"+YHuH+6RmkS5sXrzoOHBxvdOO0zASvJGwiBWwNDm4H6JwQTaAeEFIfAys93ZPuxs\n"\
"txqlpLH2sX0eaiNNRfBko99Uo/W0McvrPDC1wZ8ix3cQ4bnWILDS\n"\
"-----END RSA PRIVATE KEY-----\n"

/* The constants above are set to const char * pointers defined in aws_demo_runner.c,
 * and externed here for use in C files.  NOTE!  THIS IS DONE FOR CONVENIENCE
 * DURING AN EVALUATION PHASE AND IS NOT GOOD PRACTICE FOR PRODUCTION SYSTEMS 
 * WHICH MUST STORE KEYS SECURELY. */
extern const char clientcredentialCLIENT_CERTIFICATE_PEM[];
extern const char* clientcredentialJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM;
extern const char clientcredentialCLIENT_PRIVATE_KEY_PEM[];
extern const uint32_t clientcredentialCLIENT_CERTIFICATE_LENGTH;
extern const uint32_t clientcredentialCLIENT_PRIVATE_KEY_LENGTH;

#endif /* AWS_CLIENT_CREDENTIAL_KEYS_H */
