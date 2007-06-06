ATK.ShuttleRelation = Class.create();
ATK.ShuttleRelation.prototype = {
  name: 'atkshuttlerelation',
  leftShuttleBox: null,
  leftShuttleOptions: null,

  rightShuttleBox: null,
  rightShuttleOptions: null,

  prevsearchvalues: {},
  searchFieldLeft: null,
  searchFieldRight: null,

  initialize: function(name, leftShuttleBox, leftShuttleData, rightShuttleBox, rightShuttleData) {
    this.name = name;

    this.searchFieldLeft = $(leftShuttleBox+'_searchfield');
    this.searchFieldRight = $(rightShuttleBox+'_searchfield');

    this.leftShuttleBox = $(leftShuttleBox);
    this.leftShuttleOptions = this.getOptionsFromData(leftShuttleData);
    this.replaceOptions(this.leftShuttleBox,this.leftShuttleOptions);

    this.rightShuttleBox = $(rightShuttleBox);
    this.rightShuttleOptions = this.getOptionsFromData(rightShuttleData);
    this.replaceOptions(this.rightShuttleBox,this.rightShuttleOptions);

    Event.observe(name+'_leftToRight',    'click',    this.moveLeftToRight.bind(this));
    Event.observe(name+'_rightToLeft',    'click',    this.moveRightToLeft.bind(this));
    Event.observe(name+'_allLeftToRight', 'click',    this.moveAllRight.bind(this));
    Event.observe(name+'_allRightToLeft', 'click',    this.moveAllLeft.bind(this));

    Event.observe(leftShuttleBox,         'dblclick', this.moveLeftToRight.bind(this));
    Event.observe(rightShuttleBox,        'dblclick', this.moveRightToLeft.bind(this));

    Event.observe(this.searchFieldLeft,   'keyup',    this.searchLeft.bind(this));
    Event.observe(this.searchFieldRight,  'keyup',    this.searchRight.bind(this));
    Event.observe(leftShuttleBox,         'change',   this.searchLeft.bind(this));
    Event.observe(rightShuttleBox,        'change',   this.searchRight.bind(this));
    Event.observe(this.searchFieldLeft,   'keypress', this.disableSubmit);
    Event.observe(this.searchFieldRight,  'keypress', this.disableSubmit);
  },

  replaceOptions: function(box, options) {
    box.options.length=0;
    for (i=0;i<options.length;i++)
    {
      box.options[i] = new Option(options[i].text, options[i].value);
    }
  },

  getOptionsFromData: function(data){
    var counter=0;
    var options = new Array();
    for (var i in data)
    {
      if (typeof(data[i])!=="function")
      {
        options[counter] = new Option(data[i],i);
        counter++;
      }
    }
    return options;
  },

  disableSubmit: function(){
    return !(window.event && window.event.keyCode == 13);
  },

  selectAll: function(shuttleBox) {
    var shuttleBox = $(shuttleBox);

    for(var i=0; i<shuttleBox.options.length; i++)
    {
      shuttleBox.options[i].selected = true;
    }
    return true;
  },

  moveLeftToRight: function(){
    this.move(this.leftShuttleBox,this.leftShuttleOptions,this.rightShuttleBox,this.rightShuttleOptions);
    ATK.Tools.var_dump('Moved that shiat, now focussing on: ',this.searchFieldLeft);
    if (this.searchFieldLeft.value) this.searchFieldLeft.focus();
  },

  moveRightToLeft: function(){
    this.move(this.rightShuttleBox,this.rightShuttleOptions,this.leftShuttleBox,this.leftShuttleOptions);
    if (this.searchFieldRight.value) this.searchFieldRight.focus();
  },

  move: function(frombox,fromoptions,tobox,tooptions) {
    ATK.Tools.debug('move('+frombox+','+fromoptions+','+tobox+','+tooptions+')');
    newfromoptions = new Array();
    for(var i=0; i<frombox.options.length; i++)
    {
      if (frombox.options[i].selected)
      {
           tooptions[tooptions.length] =             new Option(frombox.options[i].text, frombox.options[i].value);
           tobox.options[tobox.options.length] =     new Option(frombox.options[i].text, frombox.options[i].value);
      }
    }

    for (var i=0;i<fromoptions.length;i++)
    {
      var found=false;
      for (var j=0;j<tooptions.length;j++)
      {
        if (tooptions[j].value===fromoptions[i].value) found=true;
      }
      if (!found) newfromoptions[newfromoptions.length]=new Option(fromoptions[i].text, fromoptions[i].value);
    }

    fromoptions.length=0;
    frombox.options.length=0
    for (i=0; i<newfromoptions.length; i++)
    {
      fromoptions[fromoptions.length] =              new Option(newfromoptions[i].text, newfromoptions[i].value);
      frombox.options[frombox.options.length] =      new Option(newfromoptions[i].text, newfromoptions[i].value);
    }
    this.searchLeft();
    this.searchRight();
  },

  moveAllLeft: function(){
    this.moveAll(this.rightShuttleBox,this.rightShuttleOptions,this.leftShuttleBox,this.leftShuttleOptions);
  },

  moveAllRight: function(){
    this.moveAll(this.leftShuttleBox,this.leftShuttleOptions,this.rightShuttleBox,this.rightShuttleOptions);
  },

  moveAll: function(fromBox,fromOptions,toBox,toOptions) {
    this.selectAll(fromBox);
    this.move(fromBox,fromOptions,toBox,toOptions);
  },

  pushOption: function(box, option) {
    ATK.Tools.var_dump(option,'pushOption');
    box.options[box.options.length] = option;
  },

  popOption: function(box) {
    if (box.length > 0)
    {
      oldoption = box.options[box.length-1];
      ATK.Tools.var_dump(oldoption,'popOption');
      box.remove(box.length - 1);
    }
    return oldoption;
  },

  empty: function(box) {
    ATK.Tools.var_dump(box,'emptying');
    box.length=0;
  },

  searchLeft: function(){
    return this.search(this.searchFieldLeft,this.leftShuttleBox,this.leftShuttleOptions);
  },

  searchRight: function(){
    return this.search(this.searchFieldRight,this.rightShuttleBox,this.rightShuttleOptions);
  },

  search: function(searchfield,selectbox,options) {
    if (this.prevsearchvalues[searchfield.id]!==searchfield.value)
    {
      this.empty(selectbox);
      if (searchfield.value)
      {
        for (i=0;i<options.length;i++)
        {
          ATK.Tools.debug(options[i].text.toLowerCase()+'.indexOf('+searchfield.value.toLowerCase()+')');
          if (options[i].text.toLowerCase().indexOf(searchfield.value.toLowerCase())!==-1)
          {
            options[i].selected=true;
            this.pushOption(selectbox, options[i]);
          }
        }
      }
      else if (options.length!=selectbox.options.length)
      {
        for (i=0;i<options.length;i++)
        {
          this.pushOption(selectbox, options[i]);
        }
      }
    }
  }
}