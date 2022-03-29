/*
 * Copyright (c) 2021 DIY356
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MG_BBINSENS_SDK_H_
#define MG_BBINSENS_SDK_H_

#include <stdbool.h>
#include "mg_bthing_sdk.h"
#include "mg_bsensor_sdk.h"
#include "mgos_bbinsens.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MG_BBINSENS_NEW(s) MG_BTHING_SENS_NEW(s);

/*****************************************
 * Cast Functions
 */

// Convert (mgos_bbinsens_t) into (struct mg_bthing_sens *)
struct mg_bthing_sens *MG_BBINSENS_CAST1(mgos_bbinsens_t thing);
/*****************************************/

#define MG_BBINSENS_CFG(s) ((struct mg_bbinsens_cfg *)MG_BBINSENS_CAST1(s)->cfg)

struct mg_bbinsens_overrides {
  mg_bthing_getting_state_handler_t getting_state_cb;
};

struct mg_bbinsens_cfg {
  struct mg_bsensor_cfg base;
  struct mg_bbinsens_overrides overrides; 
  const char *payload_true;
  const char *payload_false;
};

bool mg_bbinsens_init(mgos_bbinsens_t sens, struct mg_bbinsens_cfg *cfg);

void mg_bbinsens_reset(mgos_bbinsens_t sens);

bool mg_bbinsens_bool_to_state(mgos_bbinsens_t sens, bool bool_state, mgos_bvar_t state);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MG_BBINSENS_SDK_H_ */