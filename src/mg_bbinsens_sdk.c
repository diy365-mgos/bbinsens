#include "mgos.h"
#include "mg_bbinsens_sdk.h"

static mgos_bvar_t s_bool_state = NULL;

/*****************************************
 * Cast Functions
 */

// Convert (mgos_bbinsens_t) into (struct mg_bthing_sens *)
struct mg_bthing_sens *MG_BBINSENS_CAST1(mgos_bbinsens_t thing) {
  return (struct mg_bthing_sens *)thing;
}
/*****************************************/

enum mg_bthing_state_result mg_bbinsens_getting_state_cb(struct mg_bthing_sens *sens,
                                                         mgos_bvar_t state,
                                                         void *userdata) {
  if (sens && state) {
    struct mg_bbinsens_cfg *cfg = MG_BBINSENS_CFG(sens);
    char is_verb = (mgos_bbinsens_state_verbose(sens) ? 1 : 0);
    enum mg_bthing_state_result ret = cfg->overrides.getting_state_cb(sens, (is_verb == 1 ? s_bool_state : state), userdata);
    if (ret != MG_BTHING_STATE_RESULT_SUCCESS) return ret;
    if (mgos_bvar_get_type((is_verb == 1 ? s_bool_state : state)) == MGOS_BVAR_TYPE_BOOL) {
      if (is_verb == 0) return MG_BTHING_STATE_RESULT_SUCCESS;
      if (mg_bbinsens_bool_to_state(sens, mgos_bvar_get_bool(s_bool_state), state)) {
        return MG_BTHING_STATE_RESULT_SUCCESS;
      }
    } else {
      LOG(LL_ERROR, ("The '%s' get-state handler returned a state of type %d (%d was expected).",
        mgos_bthing_get_uid(MGOS_BBINSENS_THINGCAST(sens)), mgos_bvar_get_type(s_bool_state), MGOS_BVAR_TYPE_BOOL));
    }
  }
  return MG_BTHING_STATE_RESULT_ERROR;
}

bool mg_bbinsens_init(mgos_bbinsens_t sens, struct mg_bbinsens_cfg *cfg) {
  if (cfg) {
    if (mg_bsensor_init(sens, &cfg->base)) {
      /* initalize payloads */
      cfg->payload_false = NULL;
      cfg->payload_true = NULL;
      /* initalize overrides cfg */
      cfg->overrides.getting_state_cb = mg_bthing_on_getting_state(sens, mg_bbinsens_getting_state_cb);

      // initialize the static temporary state variable
      if (!s_bool_state) s_bool_state = mgos_bvar_new_bool(false);
      
      return true; // initialization successfully completed
    }
    mg_bbinsens_reset(sens);
  } else {
    LOG(LL_ERROR, ("Invalid NULL 'cfg' parameter."));
  }

  LOG(LL_ERROR, ("Error initializing bBinarySensor '%s'. See above error message for more details.",
    mgos_bthing_get_uid(MGOS_BBINSENS_THINGCAST(sens))));
  return false; 
}

void mg_bbinsens_reset(mgos_bbinsens_t sens) {
  struct mg_bbinsens_cfg *cfg = MG_BBINSENS_CFG(sens);

  /* clear overrides cfg */
  if (cfg->overrides.getting_state_cb) {
    mg_bthing_on_getting_state(sens, cfg->overrides.getting_state_cb);
    cfg->overrides.getting_state_cb = NULL;
  }
  /* clear payloads */
  free((char*)cfg->payload_false);
  free((char*)cfg->payload_true);

  // reset sensor-base obj
  mg_bsensor_reset(sens);
}

// OK
bool mg_bbinsens_bool_to_state(mgos_bbinsens_t sens, bool bool_state, mgos_bvar_t state) {
  if (sens) {
    if (mgos_bbinsens_state_verbose(sens)) {
      struct mg_bbinsens_cfg *cfg = MG_BBINSENS_CFG(sens);
      mgos_bvar_set_str(state, (bool_state ? cfg->payload_true : cfg->payload_false));
    } else {
      mgos_bvar_set_bool(state, bool_state);
    }
    return true;
  }
  return false;
}