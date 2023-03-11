const { Users } = require('../dbObjects');
const markdownToImg = require('../utils/mardown-to-png');
const replyWithReactionCollector = require('../utils/reply-with-reaction-collector');
const filterMessages = require('../utils/filter-mentions.js');

module.exports = {
	name: 'latex',

	description: 'Render latex message',

	async execute(message) {

		if (message.member.user.bot) {
			return;
		}
		let katexOptions;
    console.log('try find katexOptions... at latex.js')
		try {
			katexOptions = JSON.parse((await Users.findOne({ where: { user_id: message.author.id } })).katexOptions);
		}
		catch {
      console.log('cant find katexOptions... at latex.js')
			katexOptions = {};
		}
		const files = await markdownToImg(
			filterMessages(message.content),
			katexOptions,
			message.id,
		);
    console.log('markdownToImg() is ended at latex.js')
		await replyWithReactionCollector(message, { files, reply: { messageReference: message } });
	},
};
