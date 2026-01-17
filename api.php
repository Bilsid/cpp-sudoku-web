<?php
header("Cache-Control: no-cache, no-store, must-revalidate");

// Get level from URL (default to 1 if missing)
$level = isset($_GET['level']) ? intval($_GET['level']) : 1;

// Pass the level to the C++ program
// Example command: ./sudoku_gen 3
$boardData = shell_exec("./sudoku_gen $level");

echo $boardData;
?>
