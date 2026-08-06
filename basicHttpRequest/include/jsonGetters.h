#include <Arduino.h>
#include <cJSON.h>

String currencySymbol(String currCode){
  if (currCode == "USD"){
    return "$";
  } else if (currCode == "GBP"){
    return "£";
  } else{
    return "?";
  }
}

class extractData{
  public:
    extractData(const char *value){
      jsonData = cJSON_Parse(value);
    }

    cJSON * getJson(){
      return jsonData;
    }

    int getId(){
      return jsonData->child->valueint;
    }

    String getCurrency(){
      return jsonData->child->next->valuestring;
    }

    float getTotalValue(){
      return jsonData->child->next->next->valuedouble;
    }

    float getCashAvailableToTrade(){
      return jsonData->child->next->next->next->child->valuedouble;
    }

    float getCashReservedForOrders(){
      return jsonData->child->next->next->next->child->next->valuedouble;
    }

    float getCashInPies(){
      return jsonData->child->next->next->next->child->next->next->valuedouble;
    }

    float getInvestmentsCurrentValue(){
      return jsonData->child->next->next->next->next->child->valuedouble;
    }

    float getInvestmentsTotalCost(){
      return jsonData->child->next->next->next->next->child->next->valuedouble;
    }

    float getInvestmentsRealisedProfitLoss(){
      return jsonData->child->next->next->next->next->child->next->next->valuedouble;
    }

    float getInvestMentsUnrealisedProfitLoss(){
      return jsonData->child->next->next->next->next->child->next->next->next->valuedouble;
    }

    String getInstrumentName(){
      return jsonData->child->child->child->next->valuestring;
    }

    float getInstrumentWalletImpactTotalCost(){
      return jsonData->child->child->next->next->next->next->next->next->next->child->next->valuedouble;
    }
    String getWalletCurrency(){
      return jsonData->child->child->next->next->next->next->next->next->next->child->valuestring;
    }

    float getInstrumentWalletImpactCurrentValue(){
      return jsonData->child->child->next->next->next->next->next->next->next->child->next->next->valuedouble;
    }
  
  private:
    cJSON* jsonData;
};