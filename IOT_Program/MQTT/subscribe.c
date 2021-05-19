#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientSub"
//#define CLIENTID    "outTopic"
//#define TOPIC       "MQTT Examples"
#define TOPIC       "outTopic"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

#include </usr/include/mysql/mysql.h>



void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

volatile MQTTClient_deliveryToken deliveredtoken;
void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    sleep(1);
    MYSQL *con = mysql_init(NULL);

  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }

  if (mysql_real_connect(con, "localhost", "admin", "1234",
          "button_on_off", 0, NULL, 0) == NULL)
  {
      finish_with_error(con);
  }

  if (mysql_query(con, "DROP TABLE IF EXISTS ultrasound"))
  {
      finish_with_error(con);
  }

  char *sql = "CREATE TABLE ultrasound(id INT(255) PRIMARY KEY AUTO_INCREMENT, ultrasound_current_state VARCHAR(255))";
  if (mysql_query(con, sql))
  {
      finish_with_error(con);
  }

    //---
    //printf("3");
    int i;
    char* payloadptr;
    char* bb="a";
    char* cc="b";
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = message->payload;
    printf("1\n");
    printf("//12%s\n",payloadptr);
    printf("//31%s\n",bb);
    if(strcmp(payloadptr, bb)==0)
    {
        printf("2\n");
        mysql_query(con, "insert into ultrasound set id = CONCAT('STU', LPAD(LAST_INSERT_ID(), 5, '0')) ,ultrasound_current_state = 'people' ");
        //mysql_query(con, "insert into button_on_off set id = CONCAT('STU', LPAD(conn.insert_id(), 5, '0')) ,name = 'people' ");        
    }

    else if(strcmp(payloadptr, cc)==0)
    {
        printf("3\n");
        mysql_query(con, "insert into ultrasound set id = CONCAT('STU', LPAD(LAST_INSERT_ID(), 5, '0')) ,ultrasound_current_state = 'no people' ");
        //mysql_query(con, "insert into button_on_off set id = CONCAT('STU', LPAD(conn.insert_id(), 5, '0')) ,name = 'no people' ");
    }
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
        //printf("2\n");
    }
    //printf("//12%s\n",message);
    putchar('\n');
    //printf("message:%s",payloadptr);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}
int main(int argc, char* argv[])
{
    /*MYSQL *con = mysql_init(NULL);

  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }

  if (mysql_real_connect(con, "localhost", "admin", "1234",
          "testdb", 0, NULL, 0) == NULL)
  {
      finish_with_error(con);
  }

  if (mysql_query(con, "DROP TABLE IF EXISTS button_on_off"))
  {
      finish_with_error(con);
  }

  char *sql = "CREATE TABLE button_on_off(id INT(255) PRIMARY KEY AUTO_INCREMENT, name VARCHAR(255))";
  if (mysql_query(con, sql))
  {
      finish_with_error(con);
  }*/


    //----
    //printf("4");
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;
    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    //printf("66 %s",msgarrvd);
    /*if(*msgarrvd=="a")
    {
        mysql_query(con, "insert into button_on_off set id = CONCAT('STU', LPAD(LAST_INSERT_ID(), 5, '0')) ,name = 'people' ");
    }

    else if(*msgarrvd=="b")
    {
        mysql_query(con, "insert into button_on_off set id = CONCAT('STU', LPAD(LAST_INSERT_ID(), 5, '0')) ,name = 'no people' ");
    }*/
    

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    //printf("1");
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);
    //printf("2");
    do
    {
        //printf("5");
        ch = getchar();
        //printf("123%s",msgarrvd);
    } while(ch!='Q' && ch != 'q');
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
