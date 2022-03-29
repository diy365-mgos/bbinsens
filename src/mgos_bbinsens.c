#include "mgos.h"
#include "mgos_bthing.h"
#include "mg_bbinsens_sdk.h"
#include "mgos_bbinsens.h"

#ifdef MGOS_HAVE_MJS
#include "mjs.h"
#endif

mgos_bthing_t MGOS_BBINSENS_THINGCAST(mgos_bbinsens_t sensor) {
  return MG_BTHING_SENS_CAST4(MG_BBINSENS_CAST1(sensor));
}

mgos_bsensor_t MGOS_BBINSENS_DOWNCAST(mgos_bbinsens_t sensor) {
  return (mgos_bsensor_t)sensor;
}

mgos_bbinsens_t mgos_bbinsens_create(const char *id, const char *domain) {
  mgos_bbinsens_t MG_BBINSENS_NEW(sens);
  if (mg_bthing_init(MG_BTHING_SENS_CAST3(sens), id, MGOS_BBINSENS_TYPE, domain)) {
    struct mg_bbinsens_cfg *cfg = calloc(1, sizeof(struct mg_bbinsens_cfg));
    if (cfg) {
      mgos_bthing_t thing = MGOS_BBINSENS_THINGCAST(sens);
      if (mg_bbinsens_init(sens, cfg) && mg_bthing_register(thing)) {
        LOG(LL_INFO, ("bBinarySensor '%s' successfully created.", mgos_bthing_get_uid(thing)));
        return sens;
      }
      mg_bthing_reset(MG_BTHING_SENS_CAST3(sens));
    } else {
      LOG(LL_ERROR, ("Unable to allocate memory for 'mg_bbinsens_cfg'"));
    }
    free(cfg);
  }
  free(sens);
  LOG(LL_ERROR, ("Error creating bBinarySensor '%s'. See above error message for more details.'", (id ? id : "")));
  return NULL; 
}

bool mgos_bbinsens_get_state(mgos_bbinsens_t sensor, bool *state) {
  mgos_bvarc_t var_state = mgos_bthing_get_state(MGOS_BBINSENS_THINGCAST(sensor));
  if (var_state) return mgos_bbinsens_state_parse(sensor, var_state, state);
  return false;
}

bool mgos_bbinsens_state_verbose(mgos_bbinsens_t sensor) {
  if (sensor) {
    struct mg_bbinsens_cfg *cfg = MG_BBINSENS_CFG(sensor);
    if (cfg->payload_false && cfg->payload_true) return true;
  }
  return false;
}

bool mgos_bbinsens_set_verbose_state(mgos_bbinsens_t sensor,
                                    const char *payload_true,
                                    const char *payload_false) {
  if (!sensor || !payload_true || !payload_false) return false;
  struct mg_bbinsens_cfg *cfg = MG_BBINSENS_CFG(sensor);
  cfg->payload_false = strdup(payload_false);
  cfg->payload_true = strdup(payload_true);
  return true;
}

bool mgos_bbinsens_state_parse(mgos_bbinsens_t sens, mgos_bvarc_t state, bool *bool_state) {
  if (!sens || !state || !bool_state) return false;
  
  enum mgos_bvar_type state_type = mgos_bvar_get_type(state);
  if (mgos_bbinsens_state_verbose(sens)) {
    if (state_type == MGOS_BVAR_TYPE_STR) {
      const char *str_state = mgos_bvar_get_str(state);
      struct mg_bbinsens_cfg *cfg = MG_BBINSENS_CFG(sens);
      if (str_state && cfg->payload_false && (strcmp(str_state, cfg->payload_false) == 0)) {
        *bool_state = false;
        return true;
      }
      if (str_state && cfg->payload_true && (strcmp(str_state, cfg->payload_true) == 0)) {
        *bool_state = true;
        return true;
      }
      LOG(LL_ERROR, ("Unexpected string state value '%s' ('%s', or '%s' was expected).", str_state, cfg->payload_true, cfg->payload_false));
    } else {
      LOG(LL_ERROR, ("Unable to convert bVariant state to string value. Unexpected bVariant type %d (MGOS_BVAR_TYPE_STR was expected).", state_type));
    }
  } else {
    if (state_type == MGOS_BVAR_TYPE_BOOL) {
      *bool_state = mgos_bvar_get_bool(state);
      return true;
    } else {
      LOG(LL_ERROR, ("Unable to convert bVariant state to boolean value. Unexpected bVariant type %d (MGOS_BVAR_TYPE_BOOL was expected).", state_type));
    }
  }
 
  return false;
}


bool mgos_bbinsens_init() {
  return true;
}