const axios = require('axios');
const { Users } = require('./dbObjects.js');

module.exports = {
	name: 'katex',

	description: 'Retrieve katex options from gist',

	async execute(message, args) {
		try {
			const {
				data: { files },
			} = await axios.get(
				`https://api.github.com/gists/${args}`,
			);
			const katexOptions = Object.assign(
				{},
				...Object.values(files).map(file => JSON.parse(file.content)),
			);
			Users.addKatexOptions(katexOptions);
			message
				.reply('KaTeX Gist stored\n' + JSON.stringify(katexOptions))
				.then(msg => msg.delete(5000));
		}
		catch { message.reply('Wrong Gist id'); }
	},
};