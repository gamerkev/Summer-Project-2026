#include <WiFi.h>
#include <HTTPClient.h>
#include <jsonGetters.h>

class Summary
{
public:
    Summary(String encoded, WiFiClass *WiFi)
    {
        if ((*WiFi).status() == WL_CONNECTED)
        {
            HTTPClient http;
            http.begin("https://live.trading212.com/api/v0/equity/account/summary");
            http.addHeader("Authorization", "Basic " + encoded);
            httpCode = http.GET();
            if (httpCode > 0)
            {
                payload = http.getString();
                http.end();
                payloadJson = extractData(payload.c_str());
                curr = payloadJson.getCurrency();
                totalVal = payloadJson.getTotalValue();
                availableToTrade = payloadJson.getCashAvailableToTrade();
                reservedForOrders = payloadJson.getCashReservedForOrders();
                totalCost = payloadJson.getInvestmentsTotalCost();
                unrealisedProfit = payloadJson.getInvestMentsUnrealisedProfitLoss();
            }
        }
        else
        {
            httpCode = -1;
            payload = "WiFi not connected";
        }
    }
    int getHttpCode()
    {
        return httpCode;
    }
    String getPayload()
    {
        return payload;
    }
    extractData getPayloadJson()
    {
        return payloadJson;
    }
    String getCurr()
    {
        return curr;
    }
    float getTotalVal()
    {
        return totalVal;
    }
    float getAvailableToTrade()
    {
        return availableToTrade;
    }
    float getReservedForOrders()
    {
        return reservedForOrders;
    }
    float getTotalCost()
    {
        return totalCost;
    }
    float getUnrealisedProfit()
    {
        return unrealisedProfit;
    }

private:
    int httpCode;
    String payload;
    extractData payloadJson;
    String curr;
    float totalVal;
    float availableToTrade;
    float reservedForOrders;
    float totalCost;
    float unrealisedProfit;
};