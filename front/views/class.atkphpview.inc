<?php
atkimport('atk.front.views.support.atkphpviewrenderer');

/**
 * Front controller view which uses plain PHP to render templates.
 *
 * @author Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
class atkPHPView extends atkView 
{
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
    return atkPHPFrontViewRenderer::inline($template, $vars, $this->getHelperTags(), $this->getHelperBlocks());
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
    return atkPHPFrontViewRenderer::file($filename, $vars, $this->getHelperTags(), $this->getHelperBlocks());    
  }
}