
// Key definitions.
KEY_DOWN  = 40;
KEY_RIGHT = 39;
KEY_UP    = 38;
KEY_LEFT  = 37;

// Detect if we are dealing with netscape/mozilla.
var ver = navigator.appVersion; 
var len = ver.length;
for (var iln = 0; iln < len; iln++) 
{
  if (ver.charAt(iln) == "(") break;
}

var isNetscape = (ver.charAt(iln+1).toUpperCase() != "C");

var keyListeners = new Array();
var focussedListener = -1;

/**
 * The generic keylistener base class.
 */

function atkGKeyListener()
{  
}

atkGKeyListener.prototype.handleKey = function(key, ctrl, shift)
{
  alert("you pressed "+key+" ctrl: "+ctrl+" shift: "+shift);
}

atkGKeyListener.prototype.focus = function()
{
  // Default listener does not know how to receive focus.
}

/**
 * The keylistener class for form elements.
 */
function atkFEKeyListener(elementId, onUp, onDown, onLeft, onRight, onCtrl)
{
  this.elementId = elementId;
  this.onUp = onUp;
  this.onDown = onDown;
  this.onLeft = onLeft;
  this.onRight = onRight;
  this.onCtrl = onCtrl;
}

atkFEKeyListener.prototype = new atkGKeyListener();
atkFEKeyListener.superclass = atkGKeyListener.prototype;

atkFEKeyListener.prototype.handleKey = function(key, ctrl, shift)
{  
  if (!ctrl)
  {   
    if(key==KEY_DOWN && this.onDown) kb_focusNext();
    if(key==KEY_UP && this.onUp) kb_focusPrevious();
    if(key==KEY_LEFT && this.onLeft) kb_focusPrevious();
    if(key==KEY_RIGHT && this.onRight) kb_focusNext();
  }
  else
  {  
    if ((key==KEY_DOWN||key==KEY_RIGHT) && this.onCtrl) kb_focusNext();
    if ((key==KEY_UP  ||key==KEY_LEFT)  && this.onCtrl) kb_focusPrevious();
  }
}

atkFEKeyListener.prototype.focus = function()
{
  var el = document.getElementById(this.elementId);
  if (el)
  {
    el.focus();
  }
  else
  {
    alert('listener not attached to an element!');
  }
}

/**
 The keyboard handler
 **/

function kb_addListener(listener)
{
  keyListeners[keyListeners.length] = listener;
}

function kb_init()
{
  document.onkeydown = kb_handleKey;
}

function kb_handleKey(e)
{ // handles a keypress
  var k = 0;
  var ctrl = false;
  var shift = false;  
  
  if (focussedListener<0)
  {
    kb_focusFirst();
  }
  
  if (focussedListener>=0)    // check if it's valid after the focusFirst check.
  {
    if (isNetscape)
    {
      k = e.which;
      var mod = parseInt(e.modifiers)
      ctrl = (mod & 2)==2;
      shift = (mod & 4)==4;  
    }
    else
    {
      k = window.event.keyCode;
      ctrl = window.event.ctrlKey;
      shift = window.event.shiftKey;
    }
    
    keyListeners[focussedListener].handleKey(k, ctrl, shift);  
  }
  else
  {
    alert('nobody has the focus');
  }

}

function kb_focusFirst()
{
  if (keyListeners.length>0) focussedListener = 0;
  keyListeners[focussedListener].focus();
}

function kb_focusLast()
{
  if (keyListeners.length>0) focussedListener = keyListeners.length-1;
  keyListeners[focussedListener].focus();
}

function kb_focusPrevious()
{
  if (focussedListener>0) focussedListener--;
  else kb_focusLast();
  keyListeners[focussedListener].focus();
}

function kb_focusNext()
{
  if (focussedListener<keyListeners.length-1) focussedListener++;
  else kb_focusFirst();
  keyListeners[focussedListener].focus();
}
