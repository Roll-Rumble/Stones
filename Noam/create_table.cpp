#include "Aws.h"
#include "DynamoDBClient.h"
#include "AttributeDefinition.h"
#include "CreateTableRequest.h"
#include "KeySchemaElement.h"
#include "ProvisionedThroughput.h"
#include "ScalarAttributeType.h"
#include <iostream>
#include <time.h>

/*
g++ create_table.cpp -o create_table.o -I /Users/orlanforshaw/Documents/Ballz/Stones/build/aws-sdk-cpp-main/src/aws-cpp-sdk-core/include/aws/core -I /Users/orlanforshaw/Documents/Ballz/Stones/build/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-dynamodb/include/aws/dynamodb -I /Users/orlanforshaw/Documents/Ballz/Stones/build/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-dynamodb/include/aws/dynamodb/model
*/

#define REGION "eu-west-2" // set region for London
#define READ_CAPACITY_UNITS 1000
#define WRITE_CAPACITY_UNITS 1000

bool AwsDoc::DynamoDB::createTable(const Aws::String &tableName, // table name could be game id
                                   const Aws::String &primaryKey, // primary key could be time
                                   const Aws::Client::ClientConfiguration &clientConfiguration) {
    
    clientConfiguration.region = REGION;

    Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfiguration);

    std::cout << "Creating table " << tableName << " with time as the primary key: \"" << primaryKey << "\"." << std::endl;

    Aws::DynamoDB::Model::CreateTableRequest request;

    Aws::DynamoDB::Model::AttributeDefinition hashKey;
    hashKey.SetAttributeName(primaryKey);
    hashKey.SetAttributeType(Aws::DynamoDB::Model::ScalarAttributeType::S);
    request.AddAttributeDefinitions(hashKey);

    Aws::DynamoDB::Model::KeySchemaElement keySchemaElement;

    keySchemaElement.WithAttributeName(primaryKey).WithKeyType(Aws::DynamoDB::Model::KeyType::HASH);
    request.AddKeySchema(keySchemaElement);

    Aws::DynamoDB::Model::ProvisionedThroughput throughput;
    throughput.WithReadCapacityUnits(READ_CAPACITY_UNITS).WithWriteCapacityUnits(WRITE_CAPACITY_UNITS);
    request.SetProvisionedThroughput(throughput);
    request.SetTableName(tableName);

    const Aws::DynamoDB::Model::CreateTableOutcome &outcome = dynamoClient.CreateTable(
            request);
    if (outcome.IsSuccess()) {
        std::cout << "Table \"" << outcome.GetResult().GetTableDescription().GetTableName() << " created!" << std::endl;
    }
    else {
        std::cerr << "Failed to create table: " << outcome.GetError().GetMessage() << std::endl;
    }

    return outcome.IsSuccess();
}

bool AwsDoc::DynamoDB::deleteTable(const Aws::String &tableName,
                                   const Aws::Client::ClientConfiguration &clientConfiguration) {
    Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfiguration);

    Aws::DynamoDB::Model::DeleteTableRequest request;
    request.SetTableName(tableName);

    const Aws::DynamoDB::Model::DeleteTableOutcome &result = dynamoClient.DeleteTable(
            request);
    if (result.IsSuccess()) {
        std::cout << "The table \""
                  << result.GetResult().GetTableDescription().GetTableName()
                  << " was deleted.\n";
    }
    else {
        std::cerr << "Failed to delete table: " << result.GetError().GetMessage() << std::endl;
    }

    return result.IsSuccess();
}

bool AwsDoc::DynamoDB::putItem(const Aws::String &tableName,
                               const Aws::String &time_key,
                               const Aws::String &time_value,
                               const Aws::String &playerID_key,
                               const Aws::String &playerID_value,
                               const Aws::String &x_position_key,
                               const Aws::String &x_position_value,
                               const Aws::String &y_position_key,
                               const Aws::String &y_position_value,
                               const Aws::Client::ClientConfiguration &clientConfiguration) {
    Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfiguration);

    Aws::DynamoDB::Model::PutItemRequest putItemRequest;
    putItemRequest.SetTableName(tableName);

    putItemRequest.AddItem(time_key, Aws::DynamoDB::Model::AttributeValue().SetS(
            time_value)); // This is the hash key
    putItemRequest.AddItem(playerID_key, Aws::DynamoDB::Model::AttributeValue().SetS(
            playerID_value));
    putItemRequest.AddItem(x_position_key, Aws::DynamoDB::Model::AttributeValue().SetS(
            x_positon_value));
    putItemRequest.AddItem(y_position_key,Aws::DynamoDB::Model::AttributeValue().SetS(
            y_position_value));

    const Aws::DynamoDB::Model::PutItemOutcome outcome = dynamoClient.PutItem(
            putItemRequest);
    if (outcome.IsSuccess()) {
        std::cout << "Successfully added current x and y position associated with the player ID !" << std::endl;
    }
    else {
        std::cerr << outcome.GetError().GetMessage() << std::endl;
    }

    return outcome.IsSuccess();
}

bool AwsDoc::DynamoDB::getItem(const Aws::String &tableName,
                               const Aws::String &partitionKey,
                               const Aws::String &partitionValue,
                               const Aws::Client::ClientConfiguration &clientConfiguration) {
    Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfiguration);
    Aws::DynamoDB::Model::GetItemRequest request;

    // Set up the request.
    request.SetTableName(tableName);
    request.AddKey(partitionKey,
                   Aws::DynamoDB::Model::AttributeValue().SetS(partitionValue));

    // Retrieve the item's fields and values.
    const Aws::DynamoDB::Model::GetItemOutcome &outcome = dynamoClient.GetItem(request);
    if (outcome.IsSuccess()) {
        // Reference the retrieved fields/values.
        const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> &item = outcome.GetResult().GetItem();
        if (!item.empty()) {
            // Output each retrieved field and its value.
            for (const auto &i: item)
                std::cout << "Values: " << ": " << i.second.GetS() << std::endl;
        }
        else {
            std::cout << "No item found with the key " << partitionKey << std::endl;
        }
    }
    else {
        std::cerr << "Failed to get item: " << outcome.GetError().GetMessage();
    }

    return outcome.IsSuccess();
}

bool AwsDoc::DynamoDB::queryItems(const Aws::String &tableName,
                                  const Aws::String &partitionKey,
                                  const Aws::String &partitionValue,
                                  const Aws::String &projectionExpression, // rules of what we want to retrieve
                                  const Aws::Client::ClientConfiguration &clientConfiguration) {
    Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfiguration);
    Aws::DynamoDB::Model::QueryRequest request;

    request.SetTableName(tableName);

    if (!projectionExpression.empty()) {
        request.SetProjectionExpression(projectionExpression);
    }

    // Set query key condition expression
    request.SetKeyConditionExpression(partitionKey + "= :valueToMatch");

    // Set Expression AttributeValues
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> attributeValues;
    attributeValues.emplace(":valueToMatch", partitionValue);

    request.SetExpressionAttributeValues(attributeValues);

    bool result = true;

    // "exclusiveStartKey" is used for pagination.
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> exclusiveStartKey;
    do {
        if (!exclusiveStartKey.empty()) {
            request.SetExclusiveStartKey(exclusiveStartKey);
            exclusiveStartKey.clear();
        }
        // Perform Query operation.
        const Aws::DynamoDB::Model::QueryOutcome &outcome = dynamoClient.Query(request);
        if (outcome.IsSuccess()) {
            // Reference the retrieved items.
            const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>> &items = outcome.GetResult().GetItems();
            if (!items.empty()) {
                std::cout << "Number of items retrieved from Query: " << items.size()
                          << std::endl;
                // Iterate each item and print.
                for (const auto &item: items) {
                    std::cout
                            << "******************************************************"
                            << std::endl;
                    // Output each retrieved field and its value.
                    for (const auto &i: item)
                        std::cout << i.first << ": " << i.second.GetS() << std::endl;
                }
            }
            else {
                std::cout << "No item found in table: " << tableName << std::endl;
            }

            exclusiveStartKey = outcome.GetResult().GetLastEvaluatedKey();
        }
        else {
            std::cerr << "Failed to Query items: " << outcome.GetError().GetMessage();
            result = false;
            break;
        }
    } while (!exclusiveStartKey.empty());

    return result;
}


int main(int argc, char **argv) {

    Aws::SDKOptions options;

    Aws::InitAPI(options);
    {
        const Aws::String tableName = (argv[1]);
        const Aws::String time_key = (argv[2]);
        const Aws::String time_value = (argv[3]);
        const Aws::String x_position_key = (argv[4]);
        const Aws::String x_position_value = (argv[5]);
        const Aws::String y_position_key = (argv[6]);
        const Aws::String y_position_value = (argv[7]);

        Aws::Client::ClientConfiguration clientConfig;

        AwsDoc::DynamoDB dynamoDB;
        dynamoDB.putItem(tableName, time_key, time_value, x_position_key,
                          x_position_value, y_position_key, y_position_value, clientConfig);
    }
    Aws::ShutdownAPI(options);
    return 0;
}