
// base path, that will be used to resolve files and exclude
basePath = '';


// list of files / patterns to load in the browser
files = [


	JASMINE,
	JASMINE_ADAPTER,

	'http://ajax.googleapis.com/ajax/libs/jquery/1.10.1/jquery.min.js',
	'https://ajax.googleapis.com/ajax/libs/angularjs/1.1.5/angular.min.js',
  'javascript/controller.js',
  'javascript/directives.js', 
  'javascript/less.js',
  'mock.js',
	'test/test.js'
];


// list of files to exclude
exclude = [
  
];


// test results reporter to use
// possible values: 'dots', 'progress', 'junit'
reporters = ['progress'];

// web server port
port = 9876;

// cli runner port
runnerPort = 9100;

colors = true;

logLevel = LOG_INFO;

autoWatch = true;

browsers = ['Chrome'];

// If browser does not capture in given timeout [ms], kill it
captureTimeout = 60000;


// Continuous Integration mode
// if true, it capture browsers, run tests and exit
singleRun = false;
