/*******************************************************************************
 * Yahoo Japan RSS News rader
 * 
 *  Original Code
 *   https://www.mgo-tec.com/blog-entry-arduino-esp32-ssl-stable-root-ca.html/3
 * *****************************************************************************/
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "HttpsRssGet.h"

//取得したYahoo! Japan RSSニュース、ルート証明書を下記のフォーマットで記載する
const char* yahoo_root_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDdzCCAl+gAwIBAgIBADANBgkqhkiG9w0BAQsFADBdMQswCQYDVQQGEwJKUDEl\n" \
"MCMGA1UEChMcU0VDT00gVHJ1c3QgU3lzdGVtcyBDTy4sTFRELjEnMCUGA1UECxMe\n" \
"U2VjdXJpdHkgQ29tbXVuaWNhdGlvbiBSb290Q0EyMB4XDTA5MDUyOTA1MDAzOVoX\n" \
"DTI5MDUyOTA1MDAzOVowXTELMAkGA1UEBhMCSlAxJTAjBgNVBAoTHFNFQ09NIFRy\n" \
"dXN0IFN5c3RlbXMgQ08uLExURC4xJzAlBgNVBAsTHlNlY3VyaXR5IENvbW11bmlj\n" \
"YXRpb24gUm9vdENBMjCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANAV\n" \
"OVKxUrO6xVmCxF1SrjpDZYBLx/KWvNs2l9amZIyoXvDjChz335c9S672XewhtUGr\n" \
"zbl+dp+++T42NKA7wfYxEUV0kz1XgMX5iZnK5atq1LXaQZAQwdbWQonCv/Q4EpVM\n" \
"VAX3NuRFg3sUZdbcDE3R3n4MqzvEFb46VqZab3ZpUql6ucjrappdUtAtCms1FgkQ\n" \
"hNBqyjoGADdH5H5XTz+L62e4iKrFvlNVspHEfbmwhRkGeC7bYRr6hfVKkaHnFtWO\n" \
"ojnflLhwHyg/i/xAXmODPIMqGplrz95Zajv8bxbXH/1KEOtOghY6rCcMU/Gt1SSw\n" \
"awNQwS08Ft1ENCcadfsCAwEAAaNCMEAwHQYDVR0OBBYEFAqFqXdlBZh8QIH4D5cs\n" \
"OPEK7DzPMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3\n" \
"DQEBCwUAA4IBAQBMOqNErLlFsceTfsgLCkLfZOoc7llsCLqJX2rKSpWeeo8HxdpF\n" \
"coJxDjrSzG+ntKEju/Ykn8sX/oymzsLS28yN/HH8AynBbF0zX2S2ZTuJbxh2ePXc\n" \
"okgfGT+Ok+vx+hfuzU7jBBJV1uXk3fs+BXziHV7Gp7yXT2g69ekuCkO2r1dcYmh8\n" \
"t/2jioSgrGK+KwmHNPBqAbubKVY8/gA3zyNs8U6qtnRGEmyR7jTV7JqR50S+kDFy\n" \
"1UkC9gLl9B/rfNmWVan/7Ir5mUf/NVoCqgTLiluHcSmRvaS0eg29mvVXIwAHIRc/\n" \
"SjnRBUkLp7Y3gaVdjKozXoEofKd9J+sAro03\n" \
"-----END CERTIFICATE----- \n";

// Fetch Yahoo RSS News
String https_Web_Get(   const char* host1,
                        String target_page,
                        char char_tag,
                        String Final_tag,
                        String Begin_tag,
                        String End_tag,
                        String Begin_tag2,
                        String End_tag2,
                        String Paragraph,
                        String Paragraph2 ) {

    String ret_str;

    WiFiClientSecure https_client;
    https_client.setCACert(yahoo_root_ca); //Yahooサイトのルート証明書をセットする

    if (https_client.connect(host1, 443)){
        Serial.print(host1); Serial.print(F("-------------"));
        Serial.println(F("connected"));
        Serial.println(F("-------WEB HTTPS GET Request Send"));

        String str1 = String("GET https://") + String( host1 ) + target_page + " HTTP/1.1\r\n";
        str1 += "Host: " + String( host1 ) + "\r\n";
        str1 += "User-Agent: BuildFailureDetectorESP32\r\n";
        str1 += "Connection: close\r\n\r\n"; //closeを使うと、サーバーの応答後に切断される。最後に空行必要
        str1 += "\0";

        https_client.print(str1); //client.println にしないこと。最後に改行コードをプラスして送ってしまう為
        https_client.flush(); //client出力が終わるまで待つ
        Serial.print(str1);
        Serial.flush(); //シリアル出力が終わるまで待つ

    }else{
        Serial.println(F("------connection failed"));
    }

    if(https_client){
        String dummy_str;
        uint16_t from, to;
        Serial.println(F("-------WEB HTTPS Response Receive"));

        while(https_client.connected()) {
            while(https_client.available()) {
                if(dummy_str.indexOf(Final_tag) == -1) {
                    dummy_str = https_client.readStringUntil(char_tag);

                    //<title>
                    if(dummy_str.indexOf(Begin_tag) >= 0) {
                        from = dummy_str.indexOf(Begin_tag) + Begin_tag.length();
                        to = dummy_str.indexOf(End_tag);
                        ret_str += Paragraph;
                        ret_str += dummy_str.substring(from,to);
                        ret_str += "  ";
                    }

                    //<description>
                    if(dummy_str.indexOf(Begin_tag2) >= 0) {
                        from = dummy_str.indexOf(Begin_tag2) + Begin_tag2.length();
                        to = dummy_str.indexOf(End_tag2);
                        //Yahoo!で始まる固定文字列の<description>は無視する
                        if( !dummy_str.substring(from,to).startsWith("Yahoo!") ) {
                            ret_str += Paragraph2;
                            ret_str += dummy_str.substring(from,to);
                            ret_str += "  ";
                        }
                    }

                }else {
                    while(https_client.available()) {
                        https_client.read(); //サーバーから送られてきた文字を１文字も余さず受信し切ることが大事
                        //delay(1);
                    }
                    delay(10);
                    https_client.stop(); //特に重要。コネクションが終わったら必ず stop() しておかないとヒープメモリを食い尽くしてしまう。
                    delay(10);
                    Serial.println(F("-------Client Stop"));

                    break;
                }
            }
        }
    }

    ret_str += "\0";

    ret_str.replace("&amp;","&");   //XMLソースの場合、半角&が正しく表示されないので、全角に置き換える
    ret_str.replace("&#039;","\'"); //XMLソースの場合、半角アポストロフィーが正しく表示されないので置き換える
    ret_str.replace("&#39;","\'");  //XMLソースの場合、半角アポストロフィーが正しく表示されないので置き換える
    ret_str.replace("&apos;","\'"); //XMLソースの場合、半角アポストロフィーが正しく表示されないので置き換える
    ret_str.replace("&quot;","\""); //XMLソースの場合、ダブルクォーテーションが正しく表示されないので置き換える

    if(ret_str.length() < 20){
        ret_str = "※ニュース記事を取得できませんでした";
    } 

    if(https_client){
        delay(10);
        https_client.stop(); //特に重要。コネクションが終わったら必ず stop() しておかないとヒープメモリを食い尽くしてしまう。
        delay(10);
        Serial.println(F("-------Client Stop"));
    }
    Serial.flush(); //シリアル出力が終わるまで待つ

    return ret_str;
}