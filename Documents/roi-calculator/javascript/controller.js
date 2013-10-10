function funnelSection(scope) {
  var visible = false;
  var me = this;

  this.toggle = function() {
    if (!scope.$$phase) {
      scope.$apply(function() {
        me.visible = !me.visible
      })
    }
    else {
      me.visible = !me.visible
    }
    scope.$broadcast("slider-toggled")
  }
  //Keeps the megabutton pressed down when clicked
  this.getButtonClass = function() {
    return me.visible ? "active" : ""
  }
}

var app = angular.module('B2BTargetedTool', ["directives"])

function Controller($scope, $timeout) {
  $scope.introHidden = false;
  $scope.hideCallToAction = false;
  $scope.introInView = true;
  $scope.doneAnimating = false;

  $scope.topFunnel = new funnelSection($scope);
  $scope.midFunnel = new funnelSection($scope);
  $scope.bottomFunnel = new funnelSection($scope);

  var learnMorePopupDelay = 1000 //ms

  $scope.hideIntro = function() {
    $scope.introHidden = true;  
    $scope.introInView = false;
    $scope.$broadcast("intro-hidden")
  }

  $scope.$on("slider-toggled", function() {
    $timeout(function() {
      $scope.hideCallToAction = true
    }, learnMorePopupDelay)
  })
}