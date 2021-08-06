function setup() {
    canvasX = 1500;
    canvasY = 700;
    gridX = 150;
    gridY = 70;
    squareSize = 10;

    rule = 220;
    ruleSet = rule.toString(2);
    
    while(ruleSet.length < 8){
        ruleSet = 0 + ruleSet;
    }


    ruleMap = {
        "1,1,1" : ruleSet[0],
        "1,1,0" : ruleSet[1],
        "1,0,1" : ruleSet[2],
        "1,0,0" : ruleSet[3],
        "0,1,1" : ruleSet[4],
        "0,1,0" : ruleSet[5],
        "0,0,1" : ruleSet[6],
        "0,0,0" : ruleSet[7]
    };

    createCanvas (canvasX, canvasY);
    grid = new Grid(canvasX, canvasY, gridX, gridY, squareSize);

    initialState = new Array(gridX).fill(0);
    initialState[gridX / 2] = 1
    grid.setTop(initialState);
    grid.drawGrid();
}

function calculateState(){
    newState = new Array(gridX);
    for (let i = 0; i < gridX; i++){
        newState[i] = calculateNeighborhood(i);
    }
    grid.setTop(newState);
}


function calculateNeighborhood(index){
    if (index == 0 || index == gridX - 1){
        return 0;
    }
    searchIndex = "";
    searchIndex = grid.G(index - 1, gridY - 1) + "," + grid.G(index, gridY - 1) + "," + grid.G(index + 1, gridY - 1);
    return ruleMap[searchIndex];
}


function draw() {
    grid.drawGrid();
    calculateState();    
    tiempoInicio = millis(); 
}

