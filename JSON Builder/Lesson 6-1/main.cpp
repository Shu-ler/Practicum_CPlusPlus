#include "json_builder.h"
#include <iostream>


using namespace std;

int main() {
    json::Print(
        json::Document{
            json::Builder{}
            .StartDict()
                .Key("key1"s).Value(123)
                .Key("key2"s).Value("value2"s)
                .Key("key3"s).StartArray()
                    .Value(456)
                    .StartDict().EndDict()
                    .StartDict()
                        .Key(""s)
                        .Value(nullptr)
                    .EndDict()
                    .Value(""s)
                .EndArray()
            .EndDict()
            .Build()
        },
        cout
    );
    cout << endl;
    cout << endl; 

    json::Print(
        json::Document{
            json::Builder{}
            .Value("just a string"s)
            .Build()
        },
        cout
    );
    cout << endl;
    cout << endl;

    json::Print(
        json::Document{
            json::Builder{}
            .StartArray()
                .Value(1)
                .Value(2)
                .Value(3)
            .EndArray()
            .Build()
        },
        cout
    );
    cout << endl;
    cout << endl;

	json::Print(
		json::Document{
            json::Builder{}
            .StartDict()
                .Key("1").StartDict()
                .EndDict()
                .Key("2").Value(2)
            .EndDict()
            .Build() 
        },
	    cout
	);
}