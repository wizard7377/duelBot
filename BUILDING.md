# DuelBot (Building)

A discord bot made for playing two player games, the process of building (and general parts of interest to developers) are detailed here.

Get support or just chat in:

[Bot Showcase](https://discord.gg/KaFpXjTH)

Or the [D++](https://dpp.dev/index.html) discord server:

[D++ - The C++ Discord API Library](https://discord.gg/dpp)

## Building:

#### Dependencies:

- [Nholmann](https://json.nlohmann.me) (For JSON)
- [OpenCV](https://docs.opencv.org/4.x/d9/df8/tutorial_root.html) (For making the board images shown to the players)
- [MySQL](https://www.mysql.com/) (Storing ratings), Server and C Connector

#### Build tools:

- [CMake](https://cmake.org/)

- [Make](https://www.gnu.org/software/make/)

- C++ Compiler with support for C++20 (GCC 13 should do it)

#### Setup:

If you do **not** have the database setup, run this MySQL query:

```sql
CREATE DATABASE  IF NOT EXISTS duelData; USE duelData;
DROP TABLE IF EXISTS userGuildIds;
CREATE TABLE userGuildIds (duelId bigint NOT NULL AUTO_INCREMENT,userId bigint NOT NULL,guildId bigint DEFAULT NULL,PRIMARY KEY (duelId));
DROP TABLE IF EXISTS userGameInfo;
CREATE TABLE userGameInfo (gameUserId bigint NOT NULL AUTO_INCREMENT,userId bigint NOT NULL,gameId int DEFAULT NULL,gameRate int DEFAULT '10000',showRate tinyint(1) DEFAULT NULL,PRIMARY KEY (gameUserId),KEY userId (userId),CONSTRAINT userGameInfo_ibfk_1 FOREIGN KEY (userId) REFERENCES userGuildIds (duelId));
```

If you do **not** have  `/secrets/config.json` setup, create it as follows:

```json
{
    "BUILD" : {
        "DISCORD" : {
            "BOT_TOKEN" : "YOUR_BOT_TOKEN"
        },
        "MYSQL" : {
            "MYOP_HOST" : "YOUR_HOST_IP",
            "MYOP_USER" : "YOUR_DATABASE_USERNAME",
            "MYOP_PASS" : "YOUR_DATABASE_PASSWORD",
            "MYOP_DATA" : "duelData",
            "MYOP_PORT" : 3306,
            "MYOP_FLAG" : 0
        }
    },
    "TEST" : {
        "DISCORD" : {
            "BOT_TOKEN" : "YOUR_BOT_TOKEN"
        },
        "MYSQL" : {
            "MYOP_HOST" : "YOUR_HOST_IP",
            "MYOP_USER" : "YOUR_DATABASE_USERNAME",
            "MYOP_PASS" : "YOUR_DATABASE_PASSWORD",
            "MYOP_DATA" : "duelData",
            "MYOP_PORT" : 3306,
            "MYOP_FLAG" : 0
        }
    }
}
```

Replace the bot token, the MySQL host, user, and password with the relevant information, keep all others the same (unless a change is required to suit your local environment). A quick note, if you want to have two tokens at your disposable, just set `"TEST"` and run `cmake -DTOK_BUILD=OFF ..`

#### Building:

Building DuelBot is quite simple, just follow the instructions below:

1. Generate the directory where board image output will be stored (`mkdir output/basic`)

2. Enter build directory (`mkdir build && cd build`)

3. Get CMake to generate your Makefile (`cmake ..`)

4. Run Make, this will take a little bit, with `make`. If you want to build it faster, run `make -jn` where n is the number of jobs you want to be running in parallel (I find it best to set it to number of cores - 2). If you want to do something that has a very high probability of not working and subpar result if it does, run `make -j`, which will run all jobs in parallel, which is highly volatile and slow

5. Run `./duelBot` and enjoy!

## Note to Windows users:

I am aware the last instruction (and the article in general) are geared towards Linux, and you may ask, what is my advice for windows users? My advice, as I will say time and time again, is to use <u>Windows Subsystem for Linux</u> (**WSL**). Seriously, it will save you so much time.
