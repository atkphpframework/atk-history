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
  
function getATKSelectors(name, form)
{
  var list = new Array();
  var prefix = name + '_atkselector';

  for (var i = 0; i < form.elements.length; i++)
  {
    if (form.elements[i].name != null && form.elements[i].name.substring(0, prefix.length) == prefix)
    {
      list[list.length] = form.elements[i];
    }
  }

  return list;
}

/**
 * Updates the selection of select boxes for the record list form.
 * @param name unique recordlist name
 * @param form reference to the form object
 * @param type "all", "none" or "invert"
 */
function updateSelection(name, form, type)
{
  /* get selectors */
  var list = getATKSelectors(name, form);

  /* walk through list */
  for (var i = 0; i < list.length; i++)
  {
    if      ("all"    == type && !list[i].disabled)	list[i].checked = true;
    else if ("none"   == type && !list[i].disabled)	list[i].checked = false;
    else if ("invert" == type && !list[i].disabled)	list[i].checked = !list[i].checked;
  }
}

/**
 * Disables / enables checkboxes depending if the record supports
 * a certain action or not.
 * @param name unique recordlist name
 * @param form reference to the form object
 */
function updateSelectable(name, form)
{
  /* get selectors */
  var list = getATKSelectors(name, form);

  /* some stuff we need to know */
  var index  = form.elements[name + '_atkaction'].selectedIndex;
  var action = form.elements[name + '_atkaction'][index].value;

  /* walk through list */
  for (var i = 0; i < list.length; i++)
  {
    /* supported actions */
    var actions = eval(name + '["' + list[i].value + '"]');
    if (typeof(actions) == 'undefined') actions = new Array();

    /* contains action? */
    disabled = true;
    for (var j = 0; disabled && j < actions.length; j++)
      if (actions[j] == action) disabled = false

    /* disable */
    list[i].disabled = disabled;
    if (disabled) list[i].checked = false;
  }
}

/**
 * Because we allow embedded recordLists for 1:n relations we need a way to somehow
 * distinguish between the submit of the edit form, and the submit of the multi-record action.
 * This method uses the atkescape option to redirect the multi-record action to a level higher
 * on the session stack, which makes it possible to return to the edit form (saving updated values!)
 * @param name unique recordlist name
 * @param form reference to the form object
 * @param target where do we escape to?
 */
function atkSubmitMRA(name, form, target)
{
  /* some stuff we need to know */
  var index = form.elements[name + '_atkaction'].selectedIndex;
  if (typeof(index) == 'undefined') var atkaction = form.elements[name + '_atkaction'].value;
  else var atkaction = form.elements[name + '_atkaction'][index].value;
  
  if (atkaction == '') return;

  /* get selectors */
  var list = getATKSelectors(name, form);

  /* count selected selectors */
  var selectorLength = 0;
  
  for (var i = 0; i < list.length; i++)
  {
    if (list[i].type == 'hidden' || (!list[i].disabled && list[i].checked))
    {
      var input = document.createElement('input');
      input.setAttribute('type', 'hidden');
      input.setAttribute('name', list[i].name.substring(name.length + 1));
      input.setAttribute('value', list[i].value);
      form.appendChild(input);    

      selectorLength++;
    }
  }

  /* change atkaction and atkrecordlist values and submit form */
  if (selectorLength > 0)
  {
    if (form.atkaction == null)
    {
      var input = document.createElement('input');
      input.setAttribute('type', 'hidden');
      input.setAttribute('name', 'atkaction');      
      input.setAttribute('value', atkaction);
      form.appendChild(input);    
    }
    else 
    {
      form.atkaction.value = atkaction;
    }

    if (form.atkrecordlist == null)
    {
      var input = document.createElement('input');
      input.setAttribute('type', 'hidden');
      input.setAttribute('name', 'atkrecordlist');      
      input.setAttribute('value', name);
      form.appendChild(input);    
    }
    else 
    {
      form.atkrecordlist.value = name;
    }
    
    // default the form is build using SESSION_DEFAULT,
    // but if we submit a multi-record-action we should
    // use SESSION_NESTED instead, the difference is that
    // SESSION_NESTED increases the session level by 1, so
    // let's do so manually
    if (form.atklevel == null)
    {
      var input = document.createElement('input');
      input.setAttribute('type', 'hidden');
      input.setAttribute('name', 'atklevel');      
      input.setAttribute('value', 1);
      form.appendChild(input);    
    }
    else 
    {
      form.atklevel.value = parseInt(form.atklevel.value) + 1;
    }    
    
    globalSubmit(form);
    form.submit();
  }
}

/**
 * Because we allow embedded recordLists for 1:n relations we need a way to somehow
 * distinguish between the submit of the edit form, and the submit of the multi-record action.
 * This method uses the atkescape option to redirect the multi-record-priority action to a level higher
 * on the session stack, which makes it possible to return to the edit form (saving updated values!)
 * @param name unique recordlist name
 * @param form reference to the form object
 * @param target where do we escape to?
 */
function atkSubmitMRPA(name, form, target)
{
  /* some stuff we need to know */
  var index = form.elements[name + '_atkaction'].selectedIndex;
  if (typeof(index) == 'undefined') var atkaction = form.elements[name + '_atkaction'].value;
  else var atkaction = form.elements[name + '_atkaction'][index].value;
  
  if (atkaction == '') return;
  
  /* initial target URL */
  target += 'atkaction=' + atkaction;

  /* get selectors */
  var list = getATKSelectors(name, form);

  /* add the selectors to the target URL */
  var selectorLength = 0;
  
  for (var i = 0; i < list.length; i++)
  {
    if (list[i].selectedIndex != 0)
    {
      var priority = list[i][list[i].selectedIndex].value;
      target += '&atkselector[' + list[i][0].value + ']=' + priority;
      selectorLength++;
    }
  }
  
  /* change atkescape value and submit form */
  if (selectorLength > 0)
  {
    form.atkescape.value = target;
    globalSubmit(form);
    form.submit();
  }
}