#include <cJSON.h>

typedef enum
{
  OTHER = -1,
  GBP = 0,
  EUR = 1,
  USD = 2
} Currency;

Currency currencySymbol(String currCode)
{
  if (currCode == "GBP")
  {
    return GBP;
  }
  else if (currCode == "EUR")
  {
    return EUR;
  }
  else if (currCode == "USD")
  {
    return USD;
  }
  else
  {
    return OTHER;
  }
}

class extractData
{
public:
  extractData(const char *value)
  {
    jsonData = cJSON_Parse(value);
  }

  extractData(cJSON* json){
    jsonData = json;
  }

  extractData() = default;

  cJSON *getJson()
  {
    return jsonData;
  }

  // Account summary endpoint

  int getId()
  {
    return jsonData->child->valueint;
  }

  String getCurrency()
  {
    return jsonData->child->next->valuestring;
  }

  float getTotalValue()
  {
    return jsonData->child->next->next->valuedouble;
  }

  float getCashAvailableToTrade()
  {
    return jsonData->child->next->next->next->child->valuedouble;
  }

  float getCashReservedForOrders()
  {
    return jsonData->child->next->next->next->child->next->valuedouble;
  }

  float getCashInPies()
  {
    return jsonData->child->next->next->next->child->next->next->valuedouble;
  }

  float getInvestmentsCurrentValue()
  {
    return jsonData->child->next->next->next->next->child->valuedouble;
  }

  float getInvestmentsTotalCost()
  {
    return jsonData->child->next->next->next->next->child->next->valuedouble;
  }

  float getInvestmentsRealisedProfitLoss()
  {
    return jsonData->child->next->next->next->next->child->next->next->valuedouble;
  }

  float getInvestMentsUnrealisedProfitLoss()
  {
    return jsonData->child->next->next->next->next->child->next->next->next->valuedouble;
  }

  // Position data

  String getPositionTicker(){
    return jsonData->child->child->valuestring;
  }

  String getPositionName(){
    return jsonData->child->child->next->valuestring;
  }

  String getPositionIsin(){
    return jsonData->child->child->next->next->valuestring;
  }

  String getPositionCurr(){
    return jsonData->child->child->next->next->next->valuestring;
  }

  String getPositionCreatedAt(){
    return jsonData->child->next->valuestring;
  }

  float getPositionQuantity(){
    return jsonData->child->next->next->valuedouble;
  }

  float getPositionQuantityAvailable(){
    return jsonData->child->next->next->next->valuedouble;
  }

  float getPositionQuantityInPies(){
    return jsonData->child->next->next->next->next->valuedouble;
  }

  float getPositionCurrentPrice(){
    return jsonData->child->next->next->next->next->next->valuedouble;
  }

  float getPositionAvgPricePaid(){
    return jsonData->child->next->next->next->next->next->next->valuedouble;
  }

  String getPositionWalletCurrency(){
    return jsonData->child->next->next->next->next->next->next->next->child->valuestring;
  }

  float getPositionWalletTotalCost(){
    return jsonData->child->next->next->next->next->next->next->next->child->next->valuedouble;
  }

  float getPositionWalletCurrentValue(){
    return jsonData->child->next->next->next->next->next->next->next->child->next->next->valuedouble;
  }

  float getPositionWalletUnrealisedProfit(){
    return jsonData->child->next->next->next->next->next->next->next->child->next->next->next->valuedouble;
  }

  float getPositionWalletFxImpact(){
    return jsonData->child->next->next->next->next->next->next->next->child->next->next->next->next->valuedouble;
  }

private:
  cJSON *jsonData;
};