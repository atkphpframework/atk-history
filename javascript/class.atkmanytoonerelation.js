/**
 * This file is part of the Achievo ATK distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be 
 * included in the distribution.
 *
 * @package atk
 * @subpackage javascript
 *
 * @copyright (c)2000-2004 Ibuildings.nl BV
 * @license http://www.achievo.org/atk/licensing ATK Open Source License
 *
 * @version $Revision$
 * $Id$
 */

function mto_parse(link, value)
{
 var value_array = value.split('=');
 if(value_array[1]=='' || typeof value_array[1] == "undefined" ) return -1;
 var atkselector = value.replace("='", "_1253D_12527").replace("'", "_12527");
 return link.replace('REPLACEME', atkselector);
}

 
  
if (!window.ATK) {
  var ATK = {};
}

ATK.ManyToOneRelation = {
  /**
  * Auto-update the attribute input form using Ajax.
  */
  autoupdate: function(searchField, resultField, selectionField, field, url, afterUpdate, minimumChars) {
    var elements = Form.getElements('entryform');
    var queryComponents = new Array();

    for (var i = 0; i < elements.length; i++) {
      if (elements[i].name && elements[i].name.substring(0, 3) != 'atk') {
        var queryComponent = Form.Element.serialize(elements[i]);
        if (queryComponent)
          queryComponents.push(queryComponent);
      }
    }

    var params = queryComponents.join('&');

    if(afterUpdate != null)
    {
      new ATK.ManyToOneRelation.Autocompleter(searchField, resultField, selectionField, field, url, { paramName: 'value', parameters: params, minChars: minimumChars, frequency: 0.5, afterUpdateElement: afterUpdate});
    }
    else
    {
      new ATK.ManyToOneRelation.Autocompleter(searchField, resultField, selectionField, field, url, { paramName: 'value', parameters: params, minChars: minimumChars, frequency: 0.5});
    }
  }
};

ATK.ManyToOneRelation.Autocompleter = Class.create();
Object.extend(Object.extend(ATK.ManyToOneRelation.Autocompleter.prototype, Ajax.Autocompleter.prototype), {
  initialize: function(element, update, selection, value, url, options) {
    Ajax.Autocompleter.prototype.initialize.apply(this, new Array(element, update, url, options));
    this.selection = $(selection);
    this.value = $(value);
    if (this.options.serializeForm)
      this.options.callback = function(el, entry) { return Form.serialize(el.form) + '&' + entry; }
  },
  
  findFirstNodeByClass: function(element, className) {
    var nodes = $(element).childNodes;
    for (var i = 0; i < nodes.length; i++)
    {
      if (nodes[i].nodeType != 3 && Element.hasClassName(nodes[i], className)) return nodes[i];
      else if (nodes[i].nodeType != 3)
      {
        node = this.findFirstNodeByClass(nodes[i], className)
        if (node != null) return node;
      }
    }
    return null;
  },  

  updateElement: function(selectedElement) {
    var value = this.findFirstNodeByClass(selectedElement, 'value').innerHTML;
    var selection = this.findFirstNodeByClass(selectedElement, 'selection').innerHTML;
    
    this.value.value = value;
    this.element.value = '';
    this.element.focus();
    
    this.selection.innerHTML = selection;    
    new Effect.Highlight(this.selection);

    if (this.options.afterUpdateElement)
      this.options.afterUpdateElement(this.element, selectedElement);    
  }  
});