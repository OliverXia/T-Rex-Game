describe("B2BTargetedTool", function(){
	var scope;
	var cntrl;
	var contactBar;
	var fade, animations;
	

	beforeEach(inject(function($rootScope, $compile, $controller) {
		scope = $rootScope.$new();
		cntrl = $controller('Controller',{$scope : scope});

		contactBar = angular.element('<div class="contact-bar" show-when="hideCallToAction">');		

		// topClick = angular.element('<div id="viewport" class="viewport" ng-style="style()" resize>');
		// midClick = angular.element('<switch method="showMiddleFunnel()"></switch>');
		// bottomClick = angular.element('<switch method="showBottomFunnel()"></switch>');
		// scope.fadeToggle = false;
		// fade = angular.element('<div fade-when="fadeToggle"></div>')
		// animations = angular.element('<div remove-animation class="intro-animations"></div>')

		$compile(contactBar)(scope);
		// $compile(topClick)(scope);
		// $compile(midClick)(scope);
		// $compile(bottomClick)(scope);
		// $compile(fade)(scope);
		// $compile(animations)(scope);

		$rootScope.$digest();

	}));


	// it("test the remove-animation directive", function(){
	// 	//doesn't seem to work

	// 	var className = $(animations).attr('class');
	// 	dump(className);
	// 	expect(className).toEqual("intro-animations ng-scope");
	// 	animations.click();
	// 	var className = $(animations).attr('class');
	// 	dump(className);
	// 	expect(className).toEqual("ng-scope");

	// });

	// it('tests the fading directive', function(){

	// 	dump(fade)
	// 	dump($(fade).css("opacity"));

	// 	expect(fade.css("opacity")).toEqual('1');		


	// 	scope.$apply(function() {
	// 		scope.fadeToggle = true;
	// 	})
	// 	dump(fade)
	// 	expect(fade.hasClass("fading")).toEqual(true);

	// it('tests the resizing directive', function(){
	// 	window.resizeTo(862, 440);
	// 	expect(topClick.offsetHeight()).toEqual(828);
	// });

	it("testing the function that hides the red bar on the bottom at startup and also makes it unclickable", function () {
		scope.hideCallToAction = false;
		expect($(contactBar).css("opacity")).toEqual(0);

	});

});



