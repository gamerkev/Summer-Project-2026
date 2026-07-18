#include <Arduino.h>
#include <cJSON.h>

class ProfileDetails{
  public:
    ProfileDetails(const char *value){
      profile = cJSON_Parse(value);
    }

    int getId(){
      return profile->child->valueint;
    }

    String getCurrency(){
      return profile->child->next->valuestring;
    }

    float getTotalValue(){
      return profile->child->next->next->valuedouble;
    }

    float getCashAvailableToTrade(){
      return profile->child->next->next->next->child->valuedouble;
    }

    float getCashReservedForOrders(){
      return profile->child->next->next->next->child->next->valuedouble;
    }

    float getCashInPies(){
      return profile->child->next->next->next->child->next->next->valuedouble;
    }

    float getInvestmentsCurrentValue(){
      return profile->child->next->next->next->next->child->valuedouble;
    }

    float getInvestmentsTotalCost(){
      return profile->child->next->next->next->next->child->next->valuedouble;
    }

    float getInvestmentsRealisedProfitLoss(){
      return profile->child->next->next->next->next->child->next->next->valuedouble;
    }

    float getInvestMentsUnrealisedProfitLoss(){
      return profile->child->next->next->next->next->child->next->next->next->valuedouble;
    }
  
  private:
    cJSON* profile;
};