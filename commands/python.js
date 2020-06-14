const { writeFileSync } = require('fs');
const replyWithReactionCollector = require('../utils/reply-with-reaction-collector');
const AWS = require('aws-sdk');


module.exports = {
	name: 'python',

	description: 'Execute python message',

	async execute(message, args) {
		AWS.config.update({ accessKeyId: process.env.AWS_AccessKeyId, secretAccessKey: process.env.AWS_SecretAccessKey, region: 'eu-west-3' });
		const lambda = new AWS.Lambda();
		const params = {
			FunctionName: 'euler',
			Payload: JSON.stringify({
				message_id: message.id,
				code: args,
			}),
		};
		lambda.invoke(params, (err, data) => {
			if (err) {
				message.reply(err.stack.toString()).then(msg => msg.delete({ timeout: 10000 }));
				console.log(err, err.stack);
			}
			else {
				const payload = JSON.parse(data.Payload);
				const body = JSON.parse(payload.body);
				const content = `
**Code**
\`\`\`python
${args}
\`\`\`
**Sortie**
\`\`\`python
${body.text}
\`\`\`
							`;
				const files = body.images.map((image, index) => {
					const filename = `/tmp/${message.id}/${index}.png`;
					writeFileSync(filename, image, 'base64');
					return filename;
				});
				replyWithReactionCollector(message, content, { files, reply: message.author });
			}
		});
	},
};
