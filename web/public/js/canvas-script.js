const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");
ctx.beginPath();
const stupidlanguage = '[{"hex":"#0000ff","coord":{"x":"23","y":"2"}},{"hex":"#008000","coord":{"x":"23","y":"1"}},{"hex":"#ff0000","coord":{"x":"24","y":"14"}},{"hex":"#ffffff","coord":{"x":"24","y":"10"}},{"hex":"#ff0000","coord":{"x":"24","y":"6"}},{"hex":"#ff0000","coord":{"x":"24","y":"3"}},{"hex":"#0000ff","coord":{"x":"24","y":"2"}},{"hex":"#008000","coord":{"x":"24","y":"1"}},{"hex":"#ff0000","coord":{"x":"25","y":"14"}},{"hex":"#ff0000","coord":{"x":"25","y":"10"}},{"hex":"#ffffff","coord":{"x":"25","y":"9"}},{"hex":"#ff0000","coord":{"x":"25","y":"6"}},{"hex":"#8000","coord":{"x":"25","y":"5"}},{"hex":"#8000","coord":{"x":"25","y":"4"}},{"hex":"#ff0000","coord":{"x":"25","y":"3"}},{"hex":"#ff","coord":{"x":"25","y":"2"}},{"hex":"#8000","coord":{"x":"25","y":"1"}},{"hex":"#ff","coord":{"x":"25","y":"0"}},{"hex":"#ff0000","coord":{"x":"26","y":"14"}},{"hex":"#ff0000","coord":{"x":"26","y":"10"}},{"hex":"#ffffff","coord":{"x":"26","y":"8"}},{"hex":"#ff0000","coord":{"x":"26","y":"6"}},{"hex":"#8000","coord":{"x":"26","y":"5"}},{"hex":"#8000","coord":{"x":"26","y":"4"}},{"hex":"#ff0000","coord":{"x":"26","y":"3"}},{"hex":"#ff","coord":{"x":"26","y":"2"}},{"hex":"#8000","coord":{"x":"26","y":"1"}},{"hex":"#ff0000","coord":{"x":"27","y":"13"}},{"hex":"#ff0000","coord":{"x":"27","y":"12"}},{"hex":"#ff0000","coord":{"x":"27","y":"11"}},{"hex":"#ffffff","coord":{"x":"27","y":"7"}},{"hex":"#ff0000","coord":{"x":"27","y":"6"}},{"hex":"#ff","coord":{"x":"27","y":"5"}},{"hex":"#ff","coord":{"x":"27","y":"4"}},{"hex":"#ff0000","coord":{"x":"27","y":"3"}},{"hex":"#ff","coord":{"x":"27","y":"2"}},{"hex":"#8000","coord":{"x":"27","y":"1"}},{"hex":"#ff","coord":{"x":"27","y":"0"}},{"hex":"#ffffff","coord":{"x":"28","y":"6"}},{"hex":"#ff","coord":{"x":"28","y":"4"}},{"hex":"#ff0000","coord":{"x":"28","y":"3"}},{"hex":"#ff","coord":{"x":"28","y":"2"}},{"hex":"#8000","coord":{"x":"28","y":"1"}},{"hex":"#ff","coord":{"x":"28","y":"0"}},{"hex":"#ff0000","coord":{"x":"29","y":"6"}},{"hex":"#ffffff","coord":{"x":"29","y":"5"}},{"hex":"#ff0000","coord":{"x":"29","y":"3"}},{"hex":"#ff","coord":{"x":"29","y":"2"}},{"hex":"#8000","coord":{"x":"29","y":"1"}},{"hex":"#8000","coord":{"x":"29","y":"0"}},{"hex":"#ff0000","coord":{"x":"30","y":"6"}},{"hex":"#ff0000","coord":{"x":"30","y":"5"}},{"hex":"#ffffff","coord":{"x":"30","y":"4"}},{"hex":"#ffffff","coord":{"x":"30","y":"3"}},{"hex":"#ff0000","coord":{"x":"30","y":"2"}},{"hex":"#ff0000","coord":{"x":"30","y":"1"}},{"hex":"#ff0000","coord":{"x":"30","y":"0"}},{"hex":"#ff","coord":{"x":"31","y":"6"}},{"hex":"#ff","coord":{"x":"31","y":"5"}},{"hex":"#ffffff","coord":{"x":"31","y":"4"}},{"hex":"#ffffff","coord":{"x":"31","y":"3"}},{"hex":"#ff0000","coord":{"x":"31","y":"2"}},{"hex":"#8000","coord":{"x":"31","y":"1"}},{"hex":"#ff","coord":{"x":"31","y":"0"}}]'
const arr = JSON.parse(stupidlanguage)


arr.forEach( cheek => {
    if(cheek.hex !== "#ffffff"){
        console.log(cheek.hex)
        ctx.fillStyle = cheek.hex;
    }
    else{
        console.log(cheek.hex)
        ctx.fillStyle = "#000000"
    }
    ctx.fillRect(cheek.coord.y*40, cheek.coord.x*20, canvas.height/40, canvas.width/40); 

});
ctx.closePath();