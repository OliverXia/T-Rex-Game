
s3cp index.html s3://com-bizo-public/roi-calculator/index.html --header 'Content-Type: text/html' --acl 'public-read'

s3cp css/styles.less s3://com-bizo-public/roi-calculator/css/styles.less --header 'Content-Type: text/less' --acl 'public-read'
s3cp css/animation.less s3://com-bizo-public/roi-calculator/css/animation.less --header 'Content-Type: text/less' --acl 'public-read'

s3cp javascript/controller.js s3://com-bizo-public/roi-calculator/javascript/controller.js --header 'Content-Type: application/javascript' --acl 'public-read'
s3cp javascript/directives.js s3://com-bizo-public/roi-calculator/javascript/directives.js --header 'Content-Type: application/javascript' --acl 'public-read'
s3cp javascript/less.js s3://com-bizo-public/roi-calculator/javascript/less.js --header 'Content-Type: application/javascript' --acl 'public-read'

s3cp images/funnel-1.png s3://com-bizo-public/roi-calculator/images/funnel-1.png --header 'Content-Type: image/png' --acl 'public-read'
s3cp images/funnel-2.png s3://com-bizo-public/roi-calculator/images/funnel-2.png --header 'Content-Type: image/png' --acl 'public-read'
s3cp images/funnel-3.png s3://com-bizo-public/roi-calculator/images/funnel-3.png --header 'Content-Type: image/png' --acl 'public-read'
s3cp images/white-funnel.png s3://com-bizo-public/roi-calculator/images/white-funnel.png --header 'Content-Type: image/png' --acl 'public-read'
s3cp images/bizo-logo.png s3://com-bizo-public/roi-calculator/images/bizo-logo.png --header 'Content-Type: image/png' --acl 'public-read'
s3cp images/secondBizo.png s3://com-bizo-public/roi-calculator/images/secondBizo.png --header 'Content-Type: image/png' --acl 'public-read'

s3cp images/zuora-logo.png s3://com-bizo-public/roi-calculator/images/zuora-logo.png --header 'Content-Type: image/png' --acl 'public-read'
s3cp images/up-sell-it-logo.png s3://com-bizo-public/roi-calculator/images/up-sell-it-logo.png --header 'Content-Type: image/png' --acl 'public-read'
s3cp images/edible-arrangements-logo.png s3://com-bizo-public/roi-calculator/images/edible-arrangements-logo.png --header 'Content-Type: image/png' --acl 'public-read'
