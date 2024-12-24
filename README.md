<h1 align="center">
  <span style="color: #2172b5;">ILoveYou</span>
</h1>

<p align="center">
  <img src="https://img.shields.io/badge/Only%20Supports-Windows-0078D4" alt="Only Supports Windows">
  <img src="https://img.shields.io/badge/License-MIT-green" alt="MIT License">
  <img src="https://img.shields.io/badge/Requires-discord.js%20Selfbot%20v13-blue" alt="Requires discord.js Selfbot v13">
</p>

---
> [!CAUTION]
> This software is dangerous. Running it will steal your Discord token, compromise your account, and spread to others. Do not run it under any circumstances. It can cause serious damage to your data and privacy.

**Disclaimer:** This is a **malicious** program inspired by the ILOVEYOU worm. This project is for **educational** purposes only. Please **do not** use it to harm others or violate Discord's terms of service.

## What is it?
ILoveYou is a Discord ILOVEYOU inspired worm that spreads through DMs with your friends.

## How does it work?
When you download and run txt.rettelevol.exe (Appears exe.loveletter.txt) it will install `index.js` (The Worm Program) `node_modules` (Modules that index.js requires) `loveletter.txt` (A copy of the love letter to be opened in notepad) and `main.zip` (The zip file containing a copy of the .exe)

Next it unzips the zip files and closes Discord completely.

When the user starts Discord again, index.js will automatically steal the user's token and send it to a webhook. After it gets the path of the .exe it sends a message to all of your friends with the .exe attached.

If your friend downloads and opens the love letter it will do the exact same thing and spread.

## Where does it install?
`C:/Users/(user)/AppData/Local/Discord/app-*/modules/dicord_desktop_core-1/discord_desktop_core`

## How do i run it?
Download the latest txt.rettelevol.exe either from the releases or from main.zip in this repository.

## How do i stop it?
Simply don't open exe.loveletter.txt
Or in the index.js set:
```js
const canRun = true;
```
To:
```js
const canRun = false;
```

## Will i continue to update this?
No, but i will make a 2.0 in the future.

## Credits
- **discord.js-selfbot-v13**: This project requires the `discord.js-selfbot-v13` module, created by [Aiko-Chan-AI](https://github.com/aiko-chan-ai).
- **discord-injection**: Very helpful. [discord-injection](https://github.com/hackirby/discord-injection) by [hackirby](https://github.com/hackirby).
