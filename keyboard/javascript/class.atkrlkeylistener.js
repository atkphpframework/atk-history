var focussedRecordlist = null;

/**
 * The keylistener class for recordlists.
 */
function atkRLKeyListener(rlId, highlight, reccount)
{
  this.recordlistId = rlId;  
  this.currentrec = -1;
  this.prevcolor = '';
  this.highlight = highlight;
  this.reccount = reccount;
}

atkRLKeyListener.prototype = new atkGKeyListener();
atkRLKeyListener.superclass = atkGKeyListener.prototype;

atkRLKeyListener.prototype.handleKey = function(key, ctrl, shift)
{  
  if (key==KEY_DOWN) this.down();
  if (key==KEY_UP) this.up();
  if (key==KEY_RIGHT) alert('nextpage in rl');
  if (key==KEY_LEFT) alert('prevpage in rl');
}

atkRLKeyListener.prototype.focus = function(direction)
{  
  focussedRecordlist = this.recordlistId;
  if (direction==DIR_UP)   // We come from below.
  {  
    this.last();
  }
  else // We come from above
  {
    this.first(); // move to first record.
  }
}

atkRLKeyListener.prototype.blur = function()
{
  focussedRecordlist = null;
}

atkRLKeyListener.prototype.first = function()
{
  // We can implement a jump to the first record by setting the pointer to -1 and 
  // moving one down.
  this.currentrec=-1;
  this.down();
}

atkRLKeyListener.prototype.last = function()
{
  this.currentrec=-1; // put the pointer to nothing.
  this.up();
}

atkRLKeyListener.prototype.down = function()
{
  if (this.currentrec>=0) // a record was already selected  
  {
    prevRow = document.getElementById(this.recordlistId+'_'+this.currentrec);
    prevRow.style.backgroundColor = this.prevcolor;
    this.currentrec++;
  }
  else  
  {
    this.currentrec=0;
  }
  
  if (this.currentrec>=this.reccount) // pointer has moved beyond last record
  {
    this.currentrec = -1; // reset pointer to nothing.
    kb_focusNext(); // pass onto next element.
  }
  else
  {
    //alert('naam: '+this.recordlistId+'_'+this.currentrec);
    newRow = document.getElementById(this.recordlistId+'_'+this.currentrec);
    this.prevcolor = newRow.style.backgroundColor;
    newRow.style.backgroundColor = this.highlight;
  }
}

atkRLKeyListener.prototype.up = function()
{
  if (this.currentrec>=0) // a record was already selected  
  {
    prevRow = document.getElementById(this.recordlistId+'_'+this.currentrec);
    prevRow.style.backgroundColor = this.prevcolor;
    this.currentrec--;
  }  
  else
  {
    this.currentrec=this.reccount-1; 
  }
  
  if (this.currentrec<0) // pointer moved before first record
  {
    this.currentrec = -1; // reset pointer to nothing.
    kb_focusPrevious(); // pass onto previous element.
  }
  else
  {
    newRow = document.getElementById(this.recordlistId+'_'+this.currentrec);
    this.prevcolor = newRow.style.backgroundColor;
    newRow.style.backgroundColor = this.highlight;
  }  
}

