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

#ifndef MGOS_BBINSENS_H_
#define MGOS_BBINSENS_H_

#include <stdbool.h>
#include "mgos_bthing.h"
#include "mgos_bsensor.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mg_bthing_sens;
typedef struct mg_bthing_sens *mgos_bbinsens_t;

mgos_bthing_t MGOS_BBINSENS_THINGCAST(mgos_bbinsens_t sensor);

mgos_bsensor_t MGOS_BBINSENS_DOWNCAST(mgos_bbinsens_t sensor);

mgos_bbinsens_t mgos_bbinsens_create(const char *id, const char *domain);

bool mgos_bbinsens_get_state(mgos_bbinsens_t sensor, bool *state);

bool mgos_bbinsens_state_verbose(mgos_bbinsens_t sensor);

bool mgos_bbinsens_set_verbose_state(mgos_bbinsens_t sensor,
                                     const char *payload_true,
                                     const char *payload_false);

bool mgos_bbinsens_state_parse(mgos_bbinsens_t sensor,
                               mgos_bvarc_t state,
                               bool *out_state);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MGOS_BBINSENS_H_ */