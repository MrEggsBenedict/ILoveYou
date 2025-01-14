<h1 align="center">
  ILoveYou
</h1>

<h3 align="center">
  A Discord Worm written in JS and C
</h3>

<p align="center">
    <img src="https://img.shields.io/badge/License-MIT-blue.svg" alt="License: MIT"/>
    <img src="https://img.shields.io/badge/Platform-Windows-green.svg" alt="Platform: Windows"/>
    <img src="https://img.shields.io/badge/Uses-discord.js_selfbot_v13-red.svg" alt="Uses discord.js selfbot v13"/>
</p>

> [!caution]
> This is Malware! This Worm spreads through your Discord Direct Messages and can be malicious.

*Disclaimer: This software is for educational purposes only, and I am not responsible for any damage or misuse.*

## What is it
ILoveYou is a Discord Worm inspired by the ILOVEYOU virus from May 2000. It spreads through your Discord Direct Messages by sending your friends a *love letter*.

## What does it do?
Once the love letter (The C part of the Malware) is opened it will:
1. Download the fake love letter.
2. Open the fake love letter in the default app for opening .txt files as exe.loveletter.txt
3. Download the payload as a .zip
4. Extract the files into `C:/Users/[user]/AppData/Local/Discord/app-[app-version]/modules/discord_desktop_core-1/discord_desktop_core/`
5. Delete the `payload.zip` file and `payload` folder.
6. Once Discord is reopened, the JS part of the Malware will steal the user's token and take control of their account.
7. Get a copy of the love letter and attach it in a message sent to all of your friends saying "I love you ❤️ Please read my love letter!".
8. If a friend downloads and opens the love letter it will repeat the process and spread further.

## How do i stop it?
If you've opened it then keep Discord open and remove it.
If you've opened it but you closed and reopened Discord then close Discord completely and remove it.

## How do i remove it?
Simply go to your `discord_desktop_core` folder (`C:/Users/[user]/AppData/Local/Discord/app-[app-version]/modules/discord_desktop_core-1/discord_desktop_core/`) then delete the `node_modules` and `bin` folders, then open `index.js` and remove everything except
```js
module.exports = require("./core.asar");
```
Then restart Discord.

### Special Thanks to:
* [Hackirby](https://github.com/hackirby) for their [Discord Injection Repository](https://github.com/hackirby/discord-injection)
* [Aiko-Chan-Ai](https://github.com/aiko-chan-ai) for [discord.js-selfbot-v13](https://github.com/aiko-chan-ai/discord.js-selfbot-v13)