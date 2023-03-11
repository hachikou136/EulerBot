const md = require('markdown-it')();
const mk = require('markdown-it-katexx');
const puppeteer = require('puppeteer');
const { readFileSync } = require('fs');
const jsdom = require('jsdom');

const child = require('child_process')


module.exports = async (text, katexOptions, name) => {
  console.log('start markdowntopng main flow at markdown-to-png.js\n');
  
	md.use(mk, katexOptions);

	const dom = new jsdom.JSDOM(readFileSync('assets/template_katex.html', 'utf8'));

	const regex = /\\begin{picture}.*?\\end{picture}|\\begin{tikzpicture}.*?\\end{tikzpicture}|\\usetikzlibrary{.*?}\s*\\begin{tikzpicture}.*?\\end{tikzpicture}|\\xymatrix{(?:[^)(]+|{(?:[^)(]+|{[^)(]*})*})*}/gms;
	let result = text.replace(
		regex,
		match =>
			`![](https://i.upmath.me/svg/${escape(
				match
					.replace(/à/g, '\\`a')
					.replace(/â/g, '\\^a')
					.replace(/é/g, '\\\'e')
					.replace(/è/g, '\\`e')
					.replace(/ê/g, '\\^e')
					.replace(/î/g, '\\^i')
					.replace(/ô/g, '\\^o')
					.replace(/ù/g, '\\`u')
					.replace(/û/g, '\\^u'),
			)})`,
	);
	result = md.render(result);

	dom.window.document.querySelector('body').innerHTML = result;
  
  function findChromium() {
    return new Promise((res, rej) => {
      child.exec("nix eval nixpkgs.chromium.outPath --raw", (error, stdout, stderr) => {
        if (error) rej(error.message);
        else if (stderr) rej(stderr);
        else res(`${stdout}/bin/chromium`);
      });
    });
  }
  const chromiumPath = await findChromium();
  console.log('findChromium is ended at markdown-to-png.js: ' + chromiumPath + '\n');
  
	const browser = await puppeteer.launch({
    // headless: false,
    dumpio : true,
    // slowMo: 250, // slow down by 250ms
    executablePath: chromiumPath,
    args: [
      '--disable-gpu',
      // '--disable-dev-shm-usage',
      '--disable-setuid-sandbox',
      // '--no-first-run',
      '--no-sandbox',
      // '--no-zygote',
      '--single-process'
    ],
	});
  
  console.log('launched puppeteer');
  console.log(await browser.userAgent());
  console.log(await browser.version());
  console.log(await browser.wsEndpoint());
  console.log(await process.versions);

  // page.on('console', msg => console.log('PAGE LOG:', msg.text()));
  // await page.evaluate(() => console.log(`url is ${location.href}`));

  console.log('opening new page...\n');
	const page = await browser.newPage();
	await page.setViewport({
		width: 1920,
		height: 1080,
		deviceScaleFactor: 3,
	});

  // await page.goto('google.com');
  
	await page.goto(`data:text/html;charset=UTF-8,${encodeURIComponent(dom.serialize())}`, { waitUntil: 'networkidle0' });

	// await page.setContent(dom.serialize());
	const content = await page.$('body');
	const fileName = `/tmp/${name}.png`;
	await content.screenshot({
		path: fileName,
	});
	await browser.close();
  console.log('closed browser at markdown-to-png\n');
	return [fileName];
};

