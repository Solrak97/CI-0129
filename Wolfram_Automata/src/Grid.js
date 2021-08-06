var black = 51;
var white = 255;

class Grid {
    constructor(canvasX, canvasY, squareX, squareY, size){
        this.squareSize = size;
        this.width = squareX;
        this.height = squareY;
        

        this.grid = new Array(this.width);
        for (let i = 0; i < this.width; i++){
            this.grid[i] = new Array(this.height);
        }

        for(let i = 0; i < this.width; i++){
            for(let j = 0; j < this.height; j++){
                this.grid[i][j] = 0;
            }   
        }
    }

    G(i,j){
        return this.grid[i][j];
    }

    drawGrid(){
        for(let i = 0; i < this.width; i++){
            for(let j = 0; j < this.height; j++){
                if ( this.grid[i][j] == 1){
                    fill(black);
                }
                else{
                    fill(white);
                }
                square(i * this.squareSize, j * this.squareSize, this.squareSize);
            }   
        }
    }

    setTop(newTop){
        for(let i = 0; i < this.width; i++){
            this.grid[i][this.height - 1] = newTop[i];
        }

       this.moveBelow();
    }

    moveBelow(){
        for(let i = 0; i < this.width; i++){
            for(let j = 1; j < this.height - 1; j++){
                this.grid[i][j] = this.grid[i][j + 1];
            }
        }
    }
}