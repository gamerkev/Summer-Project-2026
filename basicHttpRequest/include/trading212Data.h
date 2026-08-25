#include <WiFi.h>
#include <HTTPClient.h>
#include <jsonGetters.h>

struct BuySellConfig // Data structure to buy/sell positions
{
    String name;
    String ticker;
    bool quantity;
    bool longHours;
    bool limitOrder;
    float costPerShare;
    float amount;
};

class Summary
{
public:
    Summary(String encoded, WiFiClass *WiFi)
    {
        if ((*WiFi).status() == WL_CONNECTED)
        {
            HTTPClient http;
            http.setReuse(false);
            http.begin("https://demo.trading212.com/api/v0/equity/account/summary");
            http.addHeader("Authorization", "Basic " + encoded);
            httpCode = http.GET();
            if (httpCode > 0)
            {
                payload = http.getString();
                http.end();
                payloadJson = ExtractData(payload.c_str());
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
    Summary()
    {
    }
    int getHttpCode()
    {
        return httpCode;
    }
    String getPayload()
    {
        return payload;
    }
    ExtractData getPayloadJson()
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
    ExtractData payloadJson;
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
    Position(String aName,
             String aWalletCurr,
             String aInstrumentCurrency,
             String aTicker,
             float aPaidPerShare,
             float aCurrShareVal,
             float aTotalMarketVal,
             float aUnrealisedProfit,
             float aSharesAvail,
             float aSharesOwned)
    {
        name = aName;
        walletCurr = aWalletCurr;
        instrumentCurrency = aInstrumentCurrency;
        ticker = aTicker;
        paidPerShare = aPaidPerShare;
        currShareVal = aCurrShareVal;
        totalMarketVal = aTotalMarketVal;
        unrealisedProfit = aUnrealisedProfit;
        sharesAvail = aSharesAvail;
        sharesOwned = aSharesOwned;
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
    String getTicker()
    {
        return ticker;
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
    float getSharesOwned()
    {
        return sharesOwned;
    }

private:
    String name;
    String walletCurr;
    String instrumentCurrency;
    String ticker;
    float paidPerShare;
    float currShareVal;
    float totalMarketVal;
    float unrealisedProfit;
    float sharesAvail;
    float sharesOwned;
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
Positions *makePositions(cJSON *payloadJson, bool *created) // Creates entire positions linked list
{
    Positions *retVal = new Positions();                // Initialise linked list
    int numPositions = cJSON_GetArraySize(payloadJson); // Get the number of positions
    (*retVal).count = numPositions;
    ExtractData currPos = ExtractData(cJSON_GetArrayItem(payloadJson, 0)); // Get the first position, head of the linked list
    (*retVal).currentPos = Position(currPos.getPositionName(),             // Create first position
                                    currPos.getPositionWalletCurrency(),
                                    currPos.getPositionCurr(),
                                    currPos.getPositionTicker(),
                                    currPos.getPositionAvgPricePaid(),
                                    currPos.getPositionCurrentPrice(),
                                    currPos.getPositionWalletCurrentValue(),
                                    currPos.getPositionWalletUnrealisedProfit(),
                                    currPos.getPositionQuantityAvailable(),
                                    currPos.getPositionQuantity());
    Positions *prev = retVal; // Pointer to the previous position, used in creating the linked list
    for (int i = 1; i < numPositions; i++)
    {
        currPos = ExtractData(cJSON_GetArrayItem(payloadJson, i));        // Store current position
        prev->nextPos = new Positions(Position(currPos.getPositionName(), // Create link to the current position from the previous one
                                               currPos.getPositionWalletCurrency(),
                                               currPos.getPositionCurr(),
                                               currPos.getPositionTicker(),
                                               currPos.getPositionAvgPricePaid(),
                                               currPos.getPositionCurrentPrice(),
                                               currPos.getPositionWalletCurrentValue(),
                                               currPos.getPositionWalletUnrealisedProfit(),
                                               currPos.getPositionQuantityAvailable(),
                                               currPos.getPositionQuantity()),
                                      prev, (*retVal).count - i); // Create link to the previous position from the current one, doubly linked list
        prev = prev->nextPos;                                     // Iterate to the next item in the list
    }
    *created = true;
    return retVal;
}

cJSON *getPositions(String encoded, WiFiClass *WiFi)
{
    cJSON *payloadJson;
    if ((*WiFi).status() == WL_CONNECTED)
    {
        HTTPClient http;
        http.begin("https://demo.trading212.com/api/v0/equity/positions");
        http.setReuse(false);
        http.addHeader("Authorization", "Basic " + encoded);
        int httpCode = http.GET();
        Serial.println(httpCode);
        if (httpCode > 0)
        {
            String payload = http.getString();
            http.end();
            payloadJson = cJSON_Parse(payload.c_str());
        }
        else
        {
            http.end();
            return cJSON_Parse("{}");
        }
    }
    else
        return cJSON_Parse("{}");
    return payloadJson;
}

void freePositions(Positions **positions, bool *created)
{
    Positions *currPos = *positions;
    Positions *nextPos = currPos->nextPos;
    int count = (*positions)->count;
    for (int i = 0; i < count - 1; i++)
    {
        nextPos = currPos->nextPos;
        delete currPos;
        currPos = nextPos;
    }
    delete currPos;
    *created = false;
}

void placeOrder(WiFiClass *WiFi, String encoded, BuySellConfig config, bool buy)
{
    cJSON *body = cJSON_CreateObject(); // Need to send a json body with the request
    cJSON_AddBoolToObject(body, "extendedHours", config.longHours);
    cJSON_AddStringToObject(body, "ticker", config.ticker.c_str());
    switch (buy)
    {
    case true:
        switch (config.quantity)
        {
        case true:
            cJSON_AddNumberToObject(body, "quantity", config.amount);
            break;
        case false:
            cJSON_AddNumberToObject(body, "quantity", config.amount / config.costPerShare);
            break;
        }
        break;
    case false:
        switch (config.quantity)
        {
        case true:
        {
            String helperString = "-" + String(config.amount); // Kept getting floating point errors when just multiplying config.amount by -1
            cJSON_AddStringToObject(body, "quantity", helperString.c_str());
            break;
        }
        case false:
        {
            int helperInt = round(config.amount / config.costPerShare * -100); // To round off to 2dp, otherwise we get error 400 from the request
            cJSON_AddNumberToObject(body, "quantity", helperInt / 100.0);
            break;
        }
        }
        break;
    }
    if (WiFi->status() == WL_CONNECTED)
    {
        HTTPClient http;
        http.setReuse(false);
        http.begin("https://demo.trading212.com/api/v0/equity/orders/market");
        http.addHeader("Authorization", "Basic " + encoded);
        http.addHeader("Content-Type", "application/json");
        Serial.println(cJSON_Print(body));
        Serial.println(config.amount);
        Serial.println(config.amount - config.amount);
        int retVal = http.POST(cJSON_PrintUnformatted(body));
        Serial.println(retVal);
        http.end();
    }
    else
        Serial.println("Wifi not connected");
}