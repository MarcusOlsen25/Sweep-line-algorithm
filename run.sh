gcc BST.c geometry.c drawGraph.c input.c main.c queue.c -o main -lm
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

./main example.txt
if [ $? -ne 0 ]; then
    echo "Program execution failed."
    exit 1
fi

pdflatex output.tex
if [ $? -ne 0 ]; then
    echo "pdflatex compilation failed."
    exit 1
fi

evince output.pdf &
