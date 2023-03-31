/*
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 Copyright (c) 2022-2023 Cascoda Ltd
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
*/

/**
 * @file
 * 
 * KNX virtual IoT Router
 * 2023-03-30 16:22:23.105912
 * ## Application Design
 *
 * support functions:
 *
 * - app_init
 *   initializes the stack values.
 * - register_resources
 *   function that registers all endpoints,
 *   e.g. sets the GET/PUT/POST/DELETE
 *      handlers for each end point
 *
 * - main
 *   starts the stack, with the registered resources.
 *   can be compiled out with NO_MAIN
 *
 *  handlers for the implemented methods (get/put):
 *   - get_[path]
 *     function that is being called when a GET is called on [path]
 *     set the global variables in the output
 *   - put_[path]
 *     function that is being called when a PUT is called on [path]
 *     if input data is of the correct type
 *       updates the global variables
 *
 * ## stack specific defines
 * - __linux__
 *   build for Linux
 * - WIN32
 *   build for Windows
 * - OC_OSCORE
 *   oscore is enabled as compile flag
 * ## File specific defines
 * - NO_MAIN
 *   compile out the function main()
 * - INCLUDE_EXTERNAL
 *   includes header file "external_header.h", so that other tools/dependencies
 *   can be included without changing this code
 * - KNX_GUI
 *   build the GUI with console option, so that all 
 *   logging can be seen in the command window
 */
#include "oc_api.h"
#include "oc_core_res.h"
#include "api/oc_knx_fp.h"
#include "port/oc_clock.h"
#include <signal.h>
/* test purpose only; commandline reset */
#include "api/oc_knx_dev.h"
#ifdef OC_SPAKE
#include "security/oc_spake2plus.h"
#endif
#ifdef INCLUDE_EXTERNAL
/* import external definitions from header file*/
/* this file should be externally supplied */
#include "external_header.h"
#endif
#include "knx_iot_virtual_router.h"

#include <stdlib.h>

#ifdef __linux__
/** linux specific code */
#include <pthread.h>
#ifndef NO_MAIN
static pthread_mutex_t mutex;
static pthread_cond_t cv;
static struct timespec ts;
#endif /* NO_MAIN */
#endif

#include <stdio.h> /* defines FILENAME_MAX */

#define MY_NAME "KNX virtual IoT Router" /**< The name of the application */
#define APP_MAX_STRING 30

#ifdef WIN32
/** windows specific code */
#include <windows.h>
static CONDITION_VARIABLE cv; /**< event loop variable */
static CRITICAL_SECTION cs;   /**< event loop variable */
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#define btoa(x) ((x) ? "true" : "false")
volatile int quit = 0;  /**< stop variable, used by handle_signal */
bool g_reset = false;   /**< reset variable, set by commandline arguments */
char g_serial_number[20] = "00FA10019000";

/* list all object urls as defines */

/* list all parameter urls as defines */






// BOOLEAN code

/**
 * @brief function to check if the url is represented by a boolean
 *
 * @param true = url value is a boolean
 * @param false = url is not a boolean
 */
bool app_is_bool_url(char* url)
{
  return false;
}

/**
 * @brief sets the global boolean variable at the url
 *
 * @param url the url indicating the global variable
 * @param value the value to be set
 */
void app_set_bool_variable(char* url, bool value) 
{
}

/**
 * @brief retrieve the global boolean variable at the url
 *
 * @param url the url indicating the global variable
 * @return the value of the variable
 */
bool app_retrieve_bool_variable(char* url) 
{
  return false;
}

// INTEGER code

/**
 * @brief function to check if the url is represented by a integer
 *
 * @param true = url value is a integer
 * @param false = url is not a integer
 */
bool app_is_int_url(char* url)
{
  return false;
}
/**
 * @brief sets the global int variable at the url
 *
 * @param url the url indicating the global variable
 * @param value the value to be set
 */
void app_set_int_variable(char* url, int value)
{
}
void app_set_integer_variable(char* url, int value)
{
  app_set_int_variable(url, value);
}

/**
 * @brief retrieve the global integer variable at the url
 *
 * @param url the url indicating the global variable
 * @return the value of the variable
 */
int app_retrieve_int_variable(char* url)
{
  return -1;
}

// DOUBLE code

/**
 * @brief function to check if the url is represented by a double
 *
 * @param true = url value is a double
 * @param false = url is not a double
 */
bool app_is_double_url(char* url)
{
  return false;
}
/**
 * @brief sets the global double variable at the url
 *
 * @param url the url indicating the global variable
 * @param value the value to be set
 */
void app_set_double_variable(char* url, double value)
{
}
/**
 * @brief retrieve the global double variable at the url
 *
 * @param url the url indicating the global variable
 * @return the value of the variable
 */
double app_retrieve_double_variable(char* url)
{
  return -1;
}

// STRING code

/**
 * @brief function to check if the url is represented by a string
 *
 * @param true = url value is a string
 * @param false = url is not a string
 */
bool app_is_string_url(char* url)
{
  return false;
}

/**
 * @brief sets the global string variable at the url
 *
 * @param url the url indicating the global variable
 * @param value the value to be set
 */
void app_set_string_variable(char* url, char* value)
{
}

/**
 * @brief retrieve the global string variable at the url
 *
 * @param url the url indicating the global variable
 * @return the value of the variable
 */
char* app_retrieve_string_variable(char* url)
{
  return NULL;
}

// FAULT code

/**
 * @brief set the fault (boolean) variable at the url
 *
 * @param url the url indicating the fault variable
 * @param value the value of the fault variable
 */
void app_set_fault_variable(char* url, bool value)
{
}

/**
 * @brief retrieve the fault (boolean) variable at the url
 *
 * @param url the url indicating the fault variable
 * @return the value of the fault variable
 */
bool app_retrieve_fault_variable(char* url)
{
  return false;
}

// PARAMETER code

bool app_is_url_parameter(char* url)
{
  return false;
}

char* app_get_parameter_url(int index)
{
  return NULL;
}

char* app_get_parameter_name(int index)
{
  return NULL;
}

bool app_is_secure()
{
#ifdef OC_OSCORE
  return true;
#else
  return false;
#endif /* OC_OSCORE */
}

static oc_put_struct_t app_put = { NULL };

void
app_set_put_cb(oc_put_cb_t cb)
{
  app_put.cb = cb;
}

oc_put_struct_t *
oc_get_put_cb(void)
{
  return &app_put;
}

void do_put_cb(char* url) 
{
  oc_put_struct_t *my_cb = oc_get_put_cb();
  if (my_cb && my_cb->cb) {
    my_cb->cb(url);
  }
}

#ifdef __cplusplus
extern "C" {
#endif
int app_initialize_stack();
void signal_event_loop(void);
void register_resources(void);
int app_init(void);
#ifdef __cplusplus
}
#endif

// DEVBOARD code

/**
 * @brief devboard button toggle callback
 *
 */
void dev_btn_toggle_cb(char *url)
{
  printf("Handling %s\n", url);
  bool val = app_retrieve_bool_variable(url);
  if (val == true)
  {
    val = false;
  }
  else
  {
    val = true;
  }
  app_set_bool_variable(url, val);
  oc_do_s_mode_with_scope(5, url, "w");
}

/**
 * @brief s-mode response callback
 * will be called when a response is received on an s-mode read request
 *
 * @param url the url
 * @param rep the full response
 * @param rep_value the parsed value of the response
 */
void
oc_add_s_mode_response_cb(char *url, oc_rep_t *rep, oc_rep_t *rep_value)
{
  (void)rep;
  (void)rep_value;

  PRINT("oc_add_s_mode_response_cb %s\n", url);
}

/**
 * @brief function to set up the device.
 *
 * sets the:
 * - manufacturer     : cascoda
 * - serial number    : 00FA10019000
 * - base path
 * - knx spec version 
 * - hardware version : [0, 1, 3]
 * - firmware version : [0, 1, 4]
 * - hardware type    : Linux/windows
 * - device model     : KNX virtual IoT Router
 *
 */
int
app_init(void)
{
  int ret = oc_init_platform("cascoda", NULL, NULL);

  /* set the application name, version, base url, device serial number */
  ret |= oc_add_device(MY_NAME, "1.0.0", "//", g_serial_number, NULL, NULL);

  oc_device_info_t *device = oc_core_get_device_info(0);

  
  /* set the hardware version 0.1.3 */
  oc_core_set_device_hwv(0, 0, 1, 3);
  
  
  /* set the firmware version 0.1.4 */
  oc_core_set_device_fwv(0, 0, 1, 4);
  

  /* set the hardware type*/
  oc_core_set_device_hwt(0, "Linux/windows");

  /* set the model */
  oc_core_set_device_model(0, "KNX virtual IoT Router");

  oc_set_s_mode_response_cb(oc_add_s_mode_response_cb);
#define PASSWORD "U3GNY3906MSRDNG8MUR1"
#ifdef OC_SPAKE
  oc_spake_set_password(PASSWORD);


  printf("\n === QR Code: KNX:S:%s;P:%s ===\n", oc_string(device->serialnumber), PASSWORD);
#endif

  return ret;
}

/**
 * @brief returns the password
 */
char* app_get_password()
{
  return PASSWORD;
}

// data point (objects) handling



// parameters handling



/**
 * @brief register all the data point resources to the stack
 * this function registers all data point level resources:
 * - each resource path is bind to a specific function for the supported methods
 *  (GET, PUT)
 * - each resource is
 *   - secure
 *   - observable
 *   - discoverable through well-known/core
 *   - used interfaces as: dpa.xxx.yyy 
 *      - xxx : function block number
 *      - yyy : data point function number
 */
void
register_resources(void)
{

}

/**
 * @brief initiate preset for device
 * current implementation: device reset as command line argument
 * @param device_index the device identifier of the list of devices
 * @param data the supplied data.
 */
void
factory_presets_cb(size_t device_index, void *data)
{
  (void)device_index;
  (void)data;

  if (g_reset) {
    PRINT("factory_presets_cb: resetting device\n");
    oc_knx_device_storage_reset(device_index, 2);
  }
}

/**
 * @brief restart the device (application depended)
 *
 * @param device_index the device identifier of the list of devices
 * @param data the supplied data.
 */
void
restart_cb(size_t device_index, void *data)
{
  (void)device_index;
  (void)data;

  PRINT("-----restart_cb -------\n");
}

/**
 * @brief set the host name on the device (application depended)
 *
 * @param device_index the device identifier of the list of devices
 * @param host_name the host name to be set on the device
 * @param data the supplied data.
 */
void
hostname_cb(size_t device_index, oc_string_t host_name, void *data)
{
  (void)device_index;
  (void)data;

  PRINT("-----host name ------- %s\n", oc_string(host_name));
}

static oc_event_callback_retval_t send_delayed_response(void *context)
{
  oc_separate_response_t *response = (oc_separate_response_t *)context;

  if (response->active)
  {
    oc_set_separate_response_buffer(response);
    oc_send_separate_response(response, OC_STATUS_CHANGED);
    printf("Delayed response sent\n");
  }
  else
  {
    printf("Delayed response NOT active\n");
  }

  return OC_EVENT_DONE;
}

/**
 * @brief software update callback
 *
 * @param device the device index
 * @param response the instance of an internal struct that is used to track
 *       		   the state of the separate response
 * @param binary_size the full size of the binary
 * @param offset the offset of the image
 * @param payload the image data
 * @param len the length of the image data
 * @param data the user data
 */
void swu_cb(size_t device,
            oc_separate_response_t *response,
            size_t binary_size,
            size_t offset,
            uint8_t *payload,
            size_t len,
            void *data)
{
  (void)device;
  (void)binary_size;
  char filename[] = "./downloaded.bin";
  PRINT(" swu_cb %s block=%d size=%d \n", filename, (int)offset, (int)len);

  FILE *write_ptr = fopen("downloaded_bin", "ab");
  size_t r = fwrite(payload, sizeof(*payload), len, write_ptr);
  fclose(write_ptr);

  oc_set_delayed_callback(response, &send_delayed_response, 0);
}

/**
 * @brief initializes the global variables
 * for the resources 
 * for the parameters
 */
void
initialize_variables(void)
{
  /* initialize global variables for resources */
  /* if wanted read them from persistent storage */
  /* parameter variables */

}

int app_set_serial_number(char* serial_number)
{
  strncpy(g_serial_number, serial_number, 20);
  return 0;
}

int app_initialize_stack()
{
  int init;
  char *fname = "my_software_image";

  PRINT("KNX-IOT Server name : \"%s\"\n", MY_NAME);

  /* show the current working folder */
  char buff[FILENAME_MAX];
  char *retbuf = NULL;
  retbuf = GetCurrentDir(buff, FILENAME_MAX);
  if (retbuf != NULL) {
    PRINT("Current working dir: %s\n", buff);
  }

  /*
   The storage folder depends on the build system
   the folder is created in the makefile, with $target as name with _cred as
   post fix.
  */
#ifdef WIN32
  char storage[400];
  sprintf(storage,"./knx_iot_virtual_router_%s",g_serial_number);  
  PRINT("\tstorage at '%s' \n",storage);
  oc_storage_config(storage);
#else
  PRINT("\tstorage at 'knx_iot_virtual_router_creds' \n");
  oc_storage_config("./knx_iot_virtual_router_creds");
#endif
  


  /*initialize the variables */
  initialize_variables();

  /* initializes the handlers structure */
  static oc_handler_t handler = { .init = app_init,
                                  .signal_event_loop = signal_event_loop,
                                  .register_resources = register_resources,
                                  .requests_entry = NULL };

  /* set the application callbacks */
  oc_set_hostname_cb(hostname_cb, NULL);
  oc_set_restart_cb(restart_cb, NULL);
  oc_set_factory_presets_cb(factory_presets_cb, NULL);
  oc_set_swu_cb(swu_cb, (void *)fname);

  /* start the stack */
  init = oc_main_init(&handler);

  if (init < 0) {
    PRINT("oc_main_init failed %d, exiting.\n", init);
    return init;
  }

#ifdef OC_OSCORE
  PRINT("OSCORE - Enabled\n");
#else
  PRINT("OSCORE - Disabled\n");
#endif /* OC_OSCORE */

  oc_device_info_t *device = oc_core_get_device_info(0);
  PRINT("serial number: %s\n", oc_string(device->serialnumber));
  oc_endpoint_t *my_ep = oc_connectivity_get_endpoints(0);
  if (my_ep != NULL) {
    PRINTipaddr(*my_ep);
    PRINT("\n");
  }
  PRINT("Server \"%s\" running, waiting on incoming "
        "connections.\n",
        MY_NAME);
  return 0;
}

#ifdef WIN32
/**
 * @brief signal the event loop (windows version)
 * wakes up the main function to handle the next callback
 */
void
signal_event_loop(void)
{

#ifndef NO_MAIN
  WakeConditionVariable(&cv);
#endif /* NO_MAIN */
}
#endif /* WIN32 */

#ifdef __linux__
/**
 * @brief signal the event loop (Linux)
 * wakes up the main function to handle the next callback
 */
void
signal_event_loop(void)
{
#ifndef NO_MAIN
  pthread_mutex_lock(&mutex);
  pthread_cond_signal(&cv);
  pthread_mutex_unlock(&mutex);
#endif /* NO_MAIN */
}
#endif /* __linux__ */


#ifndef NO_MAIN

/**
 * @brief handle Ctrl-C
 * @param signal the captured signal
 */
static void
handle_signal(int signal)
{
  (void)signal;
  signal_event_loop();
  quit = 1;
}

/**
 * @brief print usage and quits
 *
 */
static void
print_usage()
{
  PRINT("Usage:\n");
  PRINT("no arguments : starts the server\n");
  PRINT("-help  : this message\n");
  PRINT("reset  : does an full reset of the device\n");
  PRINT("-s <serial number> : sets the serial number of the device\n");
  exit(0);
}
/**
 * @brief main application.
 * initializes the global variables
 * registers and starts the handler
 * handles (in a loop) the next event.
 * shuts down the stack
 */
int
main(int argc, char *argv[])
{
  oc_clock_time_t next_event;
  bool do_send_s_mode = false;

#ifdef KNX_GUI
  WinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWNORMAL);
#endif

#ifdef WIN32
  /* windows specific */
  InitializeCriticalSection(&cs);
  InitializeConditionVariable(&cv);
  /* install Ctrl-C */
  signal(SIGINT, handle_signal);
#endif
#ifdef __linux__
  /* Linux specific */
  struct sigaction sa;
  sigfillset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handle_signal;
  /* install Ctrl-C */
  sigaction(SIGINT, &sa, NULL);
#endif

  for (int i = 0; i < argc; i++) {
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  if (argc > 1) {
    if (strcmp(argv[1], "reset") == 0) {
      PRINT(" internal reset\n");
      g_reset = true;
    }
    if (strcmp(argv[1], "-help") == 0) {
      print_usage();
    }
  }
  if (argc > 2) {
     if (strcmp(argv[1], "-s") == 0) {
        // serial number
        PRINT("serial number %s\n", argv[2]);
        app_set_serial_number(argv[2]);
     }
  }

  /* do all initialization */
  app_initialize_stack();

#ifdef WIN32
  /* windows specific loop */
  while (quit != 1) {
    next_event = oc_main_poll();
    if (next_event == 0) {
      SleepConditionVariableCS(&cv, &cs, INFINITE);
    } else {
      oc_clock_time_t now = oc_clock_time();
      if (now < next_event) {
        SleepConditionVariableCS(
          &cv, &cs, (DWORD)((next_event - now) * 1000 / OC_CLOCK_SECOND));
      }
    }
  }
#endif

#ifdef __linux__
  /* Linux specific loop */
  while (quit != 1) {
    next_event = oc_main_poll();
    pthread_mutex_lock(&mutex);
    if (next_event == 0) {
      pthread_cond_wait(&cv, &mutex);
    } else {
      ts.tv_sec = (next_event / OC_CLOCK_SECOND);
      ts.tv_nsec = (next_event % OC_CLOCK_SECOND) * 1.e09 / OC_CLOCK_SECOND;
      pthread_cond_timedwait(&cv, &mutex, &ts);
    }
    pthread_mutex_unlock(&mutex);
  }
#endif

  /* shut down the stack */
  oc_main_shutdown();
  return 0;
}
#endif /* NO_MAIN */
