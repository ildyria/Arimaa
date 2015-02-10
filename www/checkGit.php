<?php

ini_set('max_execution_time', 900);

# Eviter les explorations abusives
define('ARIMAA',true);
# Chemin de la page
$path['root'] = dirname(__FILE__);

include $path['root'].'/includes/conf.php';

### CLASS
require_once $path['root'].'/includes/classes/simple_html_dom.class.php';

$ch=curl_init();
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1); 
curl_setopt($ch, CURLOPT_COOKIEJAR, 'cookie.txt');
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 2);
curl_setopt($ch, CURLOPT_HEADER, 1);
curl_setopt($ch, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.3; WOW64; rv:30.0) Gecko/20100101 Firefox/30.0"); 

function Connect($token,$key) {
	global $ch;
	
	curl_setopt($ch, CURLOPT_URL, 'https://www.deviantart.com/users/login');
	curl_setopt($ch, CURLOPT_POST, 1);
	curl_setopt($ch, CURLOPT_POSTFIELDS, 'username='.LOGIN.'&password='.PASSWORD);
	$store = curl_exec($ch);
	return $store;
}

function LoadURL($feed_url) {
	global $ch;

	curl_setopt($ch, CURLOPT_URL,rawurldecode($feed_url));
	curl_setopt($ch, CURLOPT_HTTPGET, 1);
	$data = curl_exec($ch);

	return $data;
}

function GetTokenAndKey($data) {
	
	$html= str_get_html($data);
	$inputs=$html->find('form[id=login] input[type=hidden]');
	$result = array();
	$result['token'] = $inputs[1]->value;
	$result['key'] = $inputs[2]->value;
	return $result;
}

function GetGallery($data,&$datasaved) {
	
	$html= str_get_html($data);
	$divgallery=$html->find('div[id=browse-results-page-1] span.details');
	foreach ($divgallery as $div) {
		$datasaved .= ($div->find('a')[0]->href)."\n";
	}

	// prevent memory leaks
	unset($divgallery);
	unset($div);
	$html->clear();
	unset($html);
};

#start loading git page

$loginpage = LoadURL('https://www.deviantart.com/users/login');
$TokenAndKey = GetTokenAndKey($loginpage);
$loged = Connect($TokenAndKey['token'],$TokenAndKey['key']);
$galleryArray = array();

while($i < $max)
{ 
	$galleryData = LoadURL('http://www.deviantart.com/?offset='.(24*$i).'&order=5&q=by%3A'.$author);
	GetGallery($galleryData,$datasaved);
	$i++;
}

curl_close($ch);
unset($ch);

file_put_contents($path['root'].'/links/'.$author.'.txt',$datasaved);

?>