Module.onRuntimeInitialized = () => {
    console.log("Emscripten runtime initialized. Initializing UI.");

    const gameBoard = document.getElementById('game-board');
    const statusText = document.getElementById('status-text');
    const resetButton = document.getElementById('reset-button');
    const themeSwitcher = document.getElementById('theme-switcher');     let cells = [];

    function createBoard() {
        gameBoard.innerHTML = '';
        cells = [];
        for (let i = 0; i < 9; i++) {
            const smallBoard = document.createElement('div');
            smallBoard.className = 'small-board';
            smallBoard.id = `small-board-${Math.floor(i / 3)}-${i % 3}`;
            for (let j = 0; j < 9; j++) {
                const cell = document.createElement('div');
                cell.className = 'cell';
                const boardI = Math.floor(i / 3);
                const boardJ = i % 3;
                const cellX = Math.floor(j / 3);
                const cellY = j % 3;
                cell.id = `cell-${boardI}-${boardJ}-${cellX}-${cellY}`;
                cell.addEventListener('click', () => {
                    Module._process_move(boardI, boardJ, cellX, cellY);
                });
                smallBoard.appendChild(cell);
                cells.push(cell);
            }
            gameBoard.appendChild(smallBoard);
        }
    }

    window.updateCell = (boardI, boardJ, cellX, cellY, player) => { 
        const cellId = `cell-${boardI}-${boardJ}-${cellX}-${cellY}`;
        const cell = document.getElementById(cellId);
        if (cell) {
            const playerChar = (player === 0) ? 'X' : 'O';
            cell.textContent = playerChar;
            cell.classList.add(playerChar);
        }
    };
    window.updateStatus = (message) => { statusText.textContent = message; };
    window.highlightNextBoard = (boardI, boardJ) => {
        document.querySelectorAll('.small-board').forEach(b => b.classList.remove('active-board'));
        if (boardI !== -1 && boardJ !== -1) {
            const boardId = `small-board-${boardI}-${boardJ}`;
            const board = document.getElementById(boardId);
            if (board) {
                board.classList.add('active-board');
            }
        }
    };
    window.markSmallBoardFinished = (boardI, boardJ, status) => { 
        const boardId = `small-board-${boardI}-${boardJ}`;
        const board = document.getElementById(boardId);
        if (board) {
            board.innerHTML = '';
            let winnerChar = '';
            if (status === 1) winnerChar = 'X';
            else if (status === 2) winnerChar = 'O';
            else if (status === 3) winnerChar = 'DRAW';
            board.textContent = winnerChar === 'DRAW' ? 'D' : winnerChar;
            board.className = `small-board won-${winnerChar}`;
        }
    };
    
    function resetGame() {
        console.log("Resetting game...");
        createBoard(); 
        Module._initialize_game(); 
    }
    resetButton.addEventListener('click', resetGame);

    function setTheme(theme) {
        document.body.setAttribute('data-theme', theme);
    }
    
    themeSwitcher.addEventListener('click', () => {
        const currentTheme = document.body.getAttribute('data-theme');
        const newTheme = currentTheme === 'light' ? 'dark' : 'light';
        setTheme(newTheme);
    });

    if (window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches) {
        setTheme('dark');
    }

    resetGame();
};