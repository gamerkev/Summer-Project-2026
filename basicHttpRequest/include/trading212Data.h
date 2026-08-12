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
            http.begin("https://demo.trading212.com/api/v0/equity/account/summary");
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

class Position // One for each position owned
{
public:
    Position(String aName, String aWalletCurr, String aInstrumentCurrency, float aPaidPerShare, float aCurrShareVal, float aTotalMarketVal, float aUnrealisedProfit, float aSharesAvail)
    {
        name = aName;
        walletCurr = aWalletCurr;
        instrumentCurrency = aInstrumentCurrency;
        paidPerShare = aPaidPerShare;
        currShareVal = aCurrShareVal;
        totalMarketVal = aTotalMarketVal;
        unrealisedProfit = aUnrealisedProfit;
        sharesAvail = aSharesAvail;
    }

    Position()
    {
    }

    String getName()
    {
        return name;
    }
    String getWalletCurrency()
    {
        return walletCurr;
    }
    String getInstrumentCurrency()
    {
        return instrumentCurrency;
    }
    float getPaidPerShare()
    {
        return paidPerShare;
    }
    float getCurrShareVal()
    {
        return currShareVal;
    }
    float getTotalMarketVal()
    {
        return totalMarketVal;
    }
    float getUnrealisedProfit()
    {
        return unrealisedProfit;
    }
    float getSharesAvail()
    {
        return sharesAvail;
    }

private:
    String name;
    String walletCurr;
    String instrumentCurrency;
    float paidPerShare;
    float currShareVal;
    float totalMarketVal;
    float unrealisedProfit;
    float sharesAvail;
};

struct Positions // Linked list structure to store positions dynamically
{
    Position currentPos;
    Positions *nextPos;
    Positions *prevPos;
    int count;
    Positions();
    Positions(Position, Positions *, int);
};
Positions::Positions() // Need empty constructor for initialising retVal
{
}
Positions::Positions(Position curr, Positions *prev, int aCount) // Used when adding a position to the linked list
{
    currentPos = curr;
    prevPos = prev;
    count = aCount;
}

Positions* makePositions(cJSON *payloadJson) // Creates entire positions linked list
{
    Positions* retVal = new Positions(); // Initialise linked list
    int numPositions = cJSON_GetArraySize(payloadJson); // Get the number of positions
    (*retVal).count = numPositions;
    extractData currPos = extractData(cJSON_GetArrayItem(payloadJson, 0)); // Get the first position, head of the linked list
    (*retVal).currentPos = Position(currPos.getPositionName(),                // Create first position
                                 currPos.getPositionWalletCurrency(),
                                 currPos.getPositionCurr(),
                                 currPos.getPositionAvgPricePaid(),
                                 currPos.getPositionCurrentPrice(),
                                 currPos.getPositionWalletCurrentValue(),
                                 currPos.getPositionWalletUnrealisedProfit(),
                                 currPos.getPositionQuantityAvailable());
    Positions *prev = retVal; // Pointer to the previous position, used in creating the linked list
    for (int i = 1; i < numPositions; i++)
    {
        currPos = extractData(cJSON_GetArrayItem(payloadJson, i));        // Store current position
        prev->nextPos = new Positions(Position(currPos.getPositionName(), // Create link to the current position from the previous one
                                               currPos.getPositionWalletCurrency(),
                                               currPos.getPositionCurr(),
                                               currPos.getPositionAvgPricePaid(),
                                               currPos.getPositionCurrentPrice(),
                                               currPos.getPositionWalletCurrentValue(),
                                               currPos.getPositionWalletUnrealisedProfit(),
                                               currPos.getPositionQuantityAvailable()),
                                      prev, (*retVal).count - i); // Create link to the previous position from the current one, doubly linked list
        prev = prev->nextPos;                                  // Iterate to the next item in the list
    }
    return retVal;
}

cJSON *getPositions(String encoded, WiFiClass *WiFi)
{
    cJSON *payloadJson;
    if ((*WiFi).status() == WL_CONNECTED)
    {
        HTTPClient http;
        http.begin("https://demo.trading212.com/api/v0/equity/positions");
        http.addHeader("Authorization", "Basic " + encoded);
        int httpCode = http.GET();
        if (httpCode > 0)
        {
            String payload = http.getString();
            http.end();
            payloadJson = cJSON_Parse(payload.c_str());
        }
    }
    return payloadJson;
}

void freePositions(Positions* positions){
    Positions* currPos = positions;
    Positions* nextPos = positions->nextPos;
    int count = positions->count;
    for (int i=0; i<count-1; i++){
        Serial.println("Freeing position " + currPos->currentPos.getName());
        nextPos = currPos->nextPos;
        Serial.println("Set netxpos in loop");
        delete currPos;
        Serial.println("Freed position " + String(i));
        currPos = nextPos;
    }
    Serial.println("Freeing final position");
    delete currPos;
}