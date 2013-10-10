var directives = angular.module('directives', [])

/*
  On window resizing we want to adjust the size of the viewport so that it
  is always large enough for the content, but not bigger than the content.
*/
directives.directive('resize', function($window) {
  return function (scope, element) {
    var win = angular.element($window)

    function heightChanging() {
      scope.$broadcast("window-resized");
    }

    scope.$on("height-changing", function(event, height) {
      adjustHeight(height)       
    })

    function adjustHeight(height) {
      element.css({ 
        height: Math.max(win.innerHeight(), height)
      })

      // trigger a ui update if not already in progress 
      if (!scope.$$phase) {
        scope.$digest()
      }
    }

    win.load(heightChanging)
    win.on('resize', heightChanging)
  }
});

/* 
 Slide up an element on a passed in event by mutating margin-top
 using a css3 transition
 ex <div slideUpOn="myEvent"></div>
 1. Event A occurs
 2. Div animates upwards
 If a window resize event occurs, update the margin-top & perform a callback 
*/
directives.directive('slideUpOn', function () {
  return {
    link: function (scope, element, attrs) {

      // applies our css3 transition rules
      element.addClass("intro-transitions")
      element.css({ "margin-top" : 0 })

      function updateIntro() {  
        var height = element.height()     
        if (scope.introHidden) {
          element.css({ 
            "margin-top" : -1 * height + "px"
          })
        }
        else {
          scope.$emit("height-changing", height)
        }
      }

      scope.$on("window-resized", function () {
        if (scope.introHidden) {
          element.css({
            "margin-top" : -1 * element.height() + "px"
          })
        }
      })

      // trigger the slide on our passed in event name 
      scope.$on(attrs.slideUpOn, function() {
        scope.introHidden = true
        scope.introInView = false
        updateIntro() 
        unRegListener();
      })

      // the viewport needs to be resized if the user changes their window size
      var unRegListener = scope.$on("window-resized", updateIntro)

      // Cross Browser Event names
      var transitionEvents = [
        "transitionend",
        "webkitTransitionEnd",
        "msTransitionEnd",
        "oTransitionEnd"
      ]
      // when the animation is over don't continue to animate changes 
      element.on(transitionEvents.join(" "), function(event) {
        if (event.target.id == "intro") { 
          scope.$emit("done-animating")
          element.removeClass("intro-transitions")
          element.css({ "display" : "none" })
        }
      })
    }
  }
})


/*
Want to send the height of the content page up to the viewport
by listening for the window-resized events and emiting back
to the directive on the viewport.
*/
directives.directive('contentResize', function () {
  return {
    link: function (scope, element, attrs) {  
      function updateContent() { 
        if(scope.introHidden && scope.doneAnimating) {
          var contentHeight = element.height()
          scope.$emit("height-changing", contentHeight)
        } 
      }
      //Checks to see if the sliding animation is done and if it is, height needs to change to accomodate the seocnd page
      scope.$on("done-animating", function() {
        scope.doneAnimating = true
        updateContent()
      })
      // listen for window-resized events and attempt to update the height of the viewport
      scope.$on("window-resized", updateContent) 
      // trigger a window-resized on the changing of the passed in attribute
      scope.$on(attrs.contentResize, function() {
        scope.$emit("window-resized")
      })
    }
  }
})

/* 
  Want to change the opacity of certain elements based on boolean values passed into the
  attribute from the scope.
*/
directives.directive("fadeWhen", function() {
  return  {
    scope: { value: "=fadeWhen" },
    link: function(scope, element, attrs) {
      element.css({
        opacity: attrs.startOpacity || 0,
        "pointer-events" : "none"
      })

      // hack to avoid a triggiering css transitions right away
      setTimeout(function() {
        element.addClass("fading")
      }, 0)
      
      scope.$watch("value", function(value) {
        if (value == true) {
          element.css({
            opacity: 1,
            "pointer-events" : "auto"
          })
        }
        else {
          element.css({
            opacity: attrs.startOpacity || 0,
            "pointer-events" : "none"
          })
        }
      })
    }
  }
});

/*
Shows the "Call to Action" button when the time is right
*/
directives.directive("showWhen", function() {
  return  {
    link: function(scope, element, attrs) { 
      scope.$watch(attrs.showWhen, function(newValue) {
        if (scope[attrs.showWhen] == true) {
          element.css({
            opacity: 1,
            "pointer-events" : "auto"
          })
        }
        else {
          element.css({
            opacity : 0
          })
        }
      })
    }
  }
});
