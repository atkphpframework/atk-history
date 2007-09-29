<?php
atkimport('atk.front.views.support.atksmartyviewwrapper');

/**
 * Front controller view which uses Smarty to render templates.
 *
 * @author Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
class atkSmartyView extends atkView 
{
  /**
   * Returns the Smarty instance.
   *
   * @return Smarty
   */
  private function getSmarty()
  {
    return atkinstance('atk.ui.atksmarty');
  }
  
  /**
   * Install smarty plug-ins for the helper view
   * tags and blocks.
   * 
   * @param Smarty $smarty
   */
  protected function installPlugins($smarty)
  {
    $tags = $this->getHelperTags();
    foreach ($tags as $name => $handler)
    {
      $name = atkFrontTools::underscore($name);
      $wrapper = new atkSmartyViewWrapper($handler);
      $smarty->register_function($name, array($wrapper, 'invokeTag'), false);
    }
    
    $blocks = $this->getHelperBlocks();
    foreach ($blocks as $name => $handler)
    {
      $name = atkFrontTools::underscore($name);
      $wrapper = new atkSmartyViewWrapper($handler);      
      $smarty->register_block($name, array($wrapper, 'invokeBlock'), false);
    }
  }  
  
  /**
   * Render inline template.
   *
   * @param string $template template
   * @param array  $vars     template variables
   * 
   * @return string rendered template
   */
  protected function renderInline($template, &$vars)
  {
    return $this->renderFile('string:'.$template, $vars);
  }
  
  /**
   * Render template file.
   *
   * @param string $filename template filename
   * @param array  $vars     template variables
   * 
   * @return string rendered template
   */
  protected function renderFile($filename, &$vars)
  {  
    $smarty = $this->getSmarty();
    
    $oldPlugins = $smarty->_plugins;
    $oldVars = $smarty->get_template_vars();
    $oldSerials = $smarty->_cache_serials;
    $oldCaching = $smarty->caching;
    $oldForceCompile = $smarty->force_compile;

    $this->installPlugins($smarty);
    $smarty->assign($vars);
    $smarty->caching = false;
    $smarty->force_compile = true;

    $result = $smarty->fetch($filename);
    $vars = $smarty->get_template_vars();
    
    $smarty->force_compile = $oldForceCompile;
    $smarty->caching = $oldCaching;
    $smarty->_cache_serials = $oldSerials;
    $smarty->_tpl_vars = $oldVars;
    $smarty->_plugins = $oldPlugins;

    return $result;
  }
}