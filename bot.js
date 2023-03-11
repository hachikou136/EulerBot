if (process.env.NODE_ENV !== 'production') require('dotenv').config();

const { readdirSync } = require('fs');
const { Replies } = require('./dbObjects');

const { Client, Intents, Collection } = require('discord.js');
const client = new Client({ intents: [Intents.FLAGS.GUILDS, Intents.FLAGS.GUILD_MESSAGES, Intents.FLAGS.GUILD_MESSAGE_REACTIONS] });
client.commands = new Collection();

const commandFiles = readdirSync('./commands').filter(file => file.endsWith('.js'));

for (const file of commandFiles) {
	const command = require(`./commands/${file}`);
	client.commands.set(command.name, command);
}

client.on('ready', () => {
	console.log('I am ready!');
});

const replyTo = async message => {
	if (message.author.bot) return;

	let commandName;
	let args;
	let match;
	match = message.content.match(/^!python[\r\n\s]+(.*?)$/s);
	if (match) {
		commandName = 'python';
		args = match[1];
	}
	match = message.content.match(/\$.*\$|\\begin|\\xymatrix/gm);
	if (match) {
		commandName = 'latex';
		args = message.content;
	}
	match = message.content.match(/^!katex (.*?)$/);
	if (match) {
		commandName = 'katex';
		args = match[1];
	}
	match = message.content.match(/^!participation$/);
	if (match) {
		commandName = 'participation';
	}

	if (!client.commands.has(commandName)) return;

	const command = client.commands.get(commandName);
  console.log('try executing command: ' + commandName)
	try {
    console.log('start executing command...')
		command.execute(message, args);
	}
	catch (error) {
		console.error(error);
		message.reply('there was an error trying to execute that command!');
	}
};

const deleteReplies = async message => {
	await Replies.findAll({ where: { reply_to_id: message.id } })
		.then(replies => replies.forEach(reply => {
			message.channel.messages.fetch(reply.message_id).then(msg => msg.delete());
		}));
	Replies.destroy({ where: { reply_to_id: message.id } });
	Replies.destroy({ where: { message_id: message.id } });
};

client.on('messageCreate', async message => {
	replyTo(message);
});

client.on('messageDelete', async message => {
	deleteReplies(message);
});

client.on('messageUpdate', async (oldMessage, newMessage) => {
	replyTo(newMessage);
	deleteReplies(newMessage);
});

client.login(process.env.BOT_TOKEN);
