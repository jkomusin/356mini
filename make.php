<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?
$keymap = $_POST['keymap'];

if(!$keymap) {
$title = "Enter the keymap content";
$body = '
<h1>Enter the keymap</h1>
<form action="make.php" method="post">
<p>
Select Chip :
<input type="radio" name="chip" value="32" checked="checked" /> Atmega32 (Default)
<input type="radio" name="chip" value="16" /> Atemga16 (for some proto)
</p>
<p>
<textarea name="keymap" style="font-family:monospace" cols="150" rows="44">
// set 0 : Normal Layer
{
	// COL0		COL1		COL2		COL3		COL4		COL5		COL6		COL7
	{ KEY_PAUSE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_RCTRL,	KEY_NONE,	KEY_LCTRL,	KEY_F5 },	// ROW0
	{ KEY_Q,	KEY_TAB,	KEY_A,		KEY_ESC,	KEY_Z,		KEY_NONE,	KEY_ESC,	KEY_1 },	// ROW1
	{ KEY_W,	KEY_CAPS,	KEY_S,		KEY_BKSLASH,KEY_X,		KEY_NONE,	KEY_F1, 	KEY_2 },	// ROW2
	{ KEY_E,	KEY_F3, 	KEY_D,		KEY_F4, 	KEY_C,		KEY_NONE,	KEY_F2, 	KEY_3 },	// ROW3
	{ KEY_R,	KEY_T,		KEY_F,		KEY_G,		KEY_V,		KEY_B,		KEY_5,		KEY_4 },	// ROW4
	{ KEY_U,	KEY_Y,		KEY_J,		KEY_H,		KEY_M,		KEY_N,		KEY_6,		KEY_7 },	// ROW5
	{ KEY_I,	KEY_RBR,	KEY_K,		KEY_F6, 	KEY_COMMA,	KEY_NONE,	KEY_EQUAL,	KEY_8 },	// ROW6
	{ KEY_O,	KEY_F7, 	KEY_L,		KEY_NONE,	KEY_DOT,	KEY_APPS,	KEY_F8, 	KEY_9 },	// ROW7
	{ KEY_P,	KEY_LBR,	KEY_COLON,	KEY_QUOTE,	KEY_NONE,	KEY_SLASH,	KEY_MINUS,	KEY_0 }, 	// ROW8
	{ KEY_SCRLCK,	KEY_NONE,	KEY_NONE,	KEY_LALT,	KEY_NONE,	KEY_RALT,	KEY_NONE,	KEY_PRNSCR },	// ROW9
	{ KEY_NONE, 	KEY_BKSP,	KEY_NONE,	KEY_F11,	KEY_ENTER,	KEY_F12,	KEY_F9, 	KEY_F10 },	// ROW10
	{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_SPACE,	KEY_NONE,	KEY_DOWN,	KEY_DEL,	KEY_NONE }, 	// ROW11
	{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_RIGHT,	KEY_INSERT, 	KEY_NONE }, 	// ROW12
	{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_PGUP,	KEY_PGDN }, 	// ROW13
	{ KEY_NONE, 	KEY_NONE,	KEY_RGUI,	KEY_UP, 	KEY_NONE,	KEY_LEFT,	KEY_HOME,	KEY_END },	// ROW14
	{ KEY_NONE, 	KEY_LSHIFT,	KEY_RSHIFT,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }, 	// ROW15
	{ KEY_NONE, 	KEY_FN,		KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }	// ROW16
}
,
// set 1 : FN Layer
{
	// COL0		COL1		COL2		COL3		COL4		COL5		COL6		COL7
	{ KEY_PAUSE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_RCTRL,	KEY_NONE,	KEY_LCTRL,	KEY_F5 },	// ROW0
	{ KEY_Q,	KEY_TAB,	KEY_A,		KEY_ESC,	KEY_Z,		KEY_NONE,	KEY_HASH,	KEY_F1 },	// ROW1
	{ KEY_W,	KEY_CAPS,	KEY_S,		KEY_INSERT,	KEY_X,		KEY_NONE,	KEY_F1, 	KEY_F2 },	// ROW2
	{ KEY_E,	KEY_F3, 	KEY_D,		KEY_F4, 	KEY_C,		KEY_NONE,	KEY_F2, 	KEY_F3 },	// ROW3
	{ KEY_R,	KEY_T,		KEY_F,		KEY_G,		KEY_V,		KEY_B,		KEY_F5,		KEY_F4 },	// ROW4
	{ KEY_U,	KEY_Y,		KEY_J,		KEY_H,		KEY_M,		KEY_N,		KEY_F6,		KEY_F7 },	// ROW5
	{ KEY_PRNSCR,	KEY_RBR,	KEY_HOME,	KEY_F6, 	KEY_END,	KEY_NONE,	KEY_F12,	KEY_F8 },	// ROW6
	{ KEY_SCRLCK,	KEY_F7, 	KEY_PGUP,	KEY_NONE,	KEY_PGDN,	KEY_APPS,	KEY_F8, 	KEY_F9 },	// ROW7
	{ KEY_PAUSE,	KEY_UP,		KEY_LEFT,	KEY_RIGHT,	KEY_NONE,	KEY_DOWN,	KEY_F11,	KEY_F10}, 	// ROW8
	{ KEY_SCRLCK,	KEY_NONE,	KEY_NONE,	KEY_LALT,	KEY_NONE,	KEY_RALT,	KEY_NONE,	KEY_PRNSCR},	// ROW9
	{ KEY_NONE, 	KEY_DEL,	KEY_NONE,	KEY_F11,	KEY_ENTER,	KEY_F12,	KEY_F9, 	KEY_F10},	// ROW10
	{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_SPACE,	KEY_NONE,	KEY_DOWN,	KEY_DEL,	KEY_NONE },	// ROW11
	{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_RIGHT,	KEY_INSERT,	KEY_NONE },	// ROW12
	{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_PGUP,	KEY_PGDN },	// ROW13
	{ KEY_NONE, 	KEY_NONE,	KEY_RGUI,	KEY_UP, 	KEY_NONE,	KEY_LEFT,	KEY_HOME,	KEY_END },	// ROW14
	{ KEY_NONE, 	KEY_LSHIFT,	KEY_RSHIFT, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE },	// ROW15
	{ KEY_NONE, 	KEY_FN,		KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }	// ROW16
}
</textarea>
</p>
<p>
<input type="submit" value="CREATE Firmware" />
</p>
</form>
<h2>Name convention</h2>
<pre style="font-family:monospace">
<strong>Special Keys</strong>
	KEY_NONE, KEY_FN

<strong>Function Part</strong>
	KEY_ESC,	KEY_F1,	KEY_F2,	KEY_F3,	KEY_F4,	KEY_F5,	KEY_F6,	KEY_F7,	KEY_F8,	KEY_F9,	KEY_F10, KEY_F11, KEY_F12,

<strong>Text Part</strong>
	KEY_HASH,	KEY_1,	KEY_2,	KEY_3,	KEY_4,	KEY_5,	KEY_6,	KEY_7,	KEY_8,	KEY_9, 	KEY_0,	KEY_MINUS,	KEY_EQUAL,	KEY_BKSP,
	KEY_TAB,	KEY_Q,	KEY_W,	KEY_E,	KEY_R,	KEY_T,	KEY_Y,	KEY_U,	KEY_I,	KEY_O,	KEY_P,	KEY_LBR,	KEY_RBR,	KEY_BKSLASH,
	KEY_CAPS,	KEY_A,	KEY_S,	KEY_D,	KEY_F,	KEY_G,	KEY_H,	KEY_J,	KEY_K,	KEY_L,	KEY_COLON,	KEY_QUOTE,	KEY_ENTER,
	KEY_LSHIFT,	KEY_Z,	KEY_X,	KEY_C,	KEY_V,	KEY_B,	KEY_N,	KEY_M,	KEY_COMMA,	KEY_DOT,	KEY_SLASH,	KEY_RSHIFT,
	KEY_LCTRL,	KEY_LGUI,	KEY_LALT,	KEY_SPACE,	KEY_RALT,	KEY_RGUI,	KEY_APPS,	KEY_RCTRL,

<strong>Number Keypad Part</strong>
	KEY_NUMLOCK,	KEY_KP_MINUS,	KEY_KP_SLASH,	KEY_KP_DOT,	KEY_KP_AST,	KEY_KP_PLUS,	KEY_KP_ENTER,
	KEY_KP_0,	KEY_KP_1,	KEY_KP_2,	KEY_KP_3,	KEY_KP_4,	KEY_KP_5,	KEY_KP_6,	KEY_KP_7,	KEY_KP_8,	KEY_KP_9,
	
<strong>Edit Key Part</strong>
	KEY_INSERT,	KEY_HOME,	KEY_PGUP,	KEY_DEL,	KEY_END,	KEY_PGDN,
	KEY_UP,	KEY_LEFT,	KEY_DOWN,	KEY_RIGHT,
	KEY_PRNSCR,	KEY_SCRLCK,	KEY_PAUSE,

<strong>Misc, Multimedia Key Part</strong>
	KEY_HANJA,	KEY_HANGLE,	KEY_POWER,	KEY_SLEEP,	KEY_WAKE,	KEY_EMAIL,
 	KEY_WWW_SEARCH, KEY_WWW_HOME,	KEY_WWW_BACK,	KEY_WWW_FORWARD,	KEY_WWW_STOP,	KEY_WWW_REFRESH,	KEY_WWW_FAVORITE,	
	KEY_NEXT_TRK,	KEY_PREV_TRK,	KEY_STOP, KEY_PLAY,	KEY_MUTE,	KEY_VOL_UP,	KEY_VOL_DOWN,	
	KEY_MEDIA,	KEY_CALC,	KEY_MYCOM,	KEY_SCREENSAVE,	KEY_REC,	KEY_REWIND,
 	KEY_MINIMIZE,	KEY_EJECT
</pre>
';
}
else {
	$chip = $_POST["chip"];

	$file = file_get_contents("keymap_template.h");
	$file = str_replace("%%%KEYMAP%%%", $keymap, $file);

	$fp = fopen("keymap.h", "w");
	
	if(flock($fp, LOCK_EX)) {
		ftruncate($fp, 0);
		fwrite($fp, $file);

		$now = date_timestamp_get(date_create());

		if($chip=="16") {
			echo "run compile_16.sh";
			$output = nl2br(shell_exec("/bin/bash compile_16.sh $now 2>&1"));
		}
		else{
			$chip="32";
			echo "run compile.sh";
			$output = nl2br(shell_exec("/bin/bash compile.sh $now 2>&1"));
		}
	
		$output = str_replace("error", "<strong style=\"color:#F00\">error</strong>", $output);
		$output = str_replace("Error", "<strong style=\"color:#F00\">Error</strong>", $output);

		
		$body = "
		<p style=\"width:80%;font-family:monospace;background-color:#EEE;margin:auto\">$output</p>
		<p> file : <a href=\"hex/keymain_$now.hex\">keymain_$now.hex</a> ( This file is temporary and will be removed in a day - so download now!)</p>
		<p> Other file needed : <br />
		<a href=\"avrdude.exe\">avrdude.exe</a> <br />
		<a href=\"avrdude.conf\">avrdude.conf</a> <br />
		script :<br />
		<strong>
		avrdude -c usbasp -P usbasp -p atmega$chip -U hfuse:w:0xC7:m -U lfuse:w:0x0E:m<br />
		avrdude -c usbasp -P usbasp -p atmega$chip -U flash:w:keymain_$now.hex:i<br /></strong>
		</p>
		<p>You entered : </p>
		<p><pre>$keymap</pre></p>
		
		";

		flock($fp, LOCK_UN);
	}
	else {
		$body="<p>Now other user is creating his/her firmware. please try later.</p><p>You entered :</p><p><pre>$keymap</pre></p>";
	}
}

//$output = shell_exec('./test.sh');
?>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
<title><?=$title ?></title>
</head>
<body>
<?=$body ?>
</body>
</html>
