<?php
global $base_url;

//dpm(get_defined_vars());

$links = $variables['links'];

$i      = 0;
$active = 0;
$first  = 0;
$last   = 0;
?>
<ul class="<?php echo $variables['classes']; ?>">
<?php
foreach($links as $key => $link) {
$active = ($key == $variables['language']) ? TRUE : FALSE;
$first  = (!$i) ? TRUE : FALSE;
$last   = ($i == count($links)) ? TRUE : FALSE;
$path   = (!empty($links[$key]['prefix'])) ? $base_url . '/' . $links[$key]['prefix'] : $base_url;
$path   = ($is_front) ? $path : $path . '/' . $links[$key]['href'];
?>
    <li class="<?php echo $key; ?><?php echo ($first) ? ' first': ''; ?><?php echo ($last) ? ' last' : ''; ?><?php echo ($active) ? ' active' : '';?>">
    <?php 
        echo l($link['title'],$link['href'],$link);
    ?>
    </li>
<?php    
    $i++;
    if($i < count($links)) {
?>
    <li class="divider">|</li>
<?php
    }

}
?>
</ul>
