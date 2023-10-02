#!/bin/bash
echo "todo's:"
echo
rg -tcpp "TODO"
echo "fixme's:"
echo
rg -tcpp "FIXME"
echo "bug's:"
echo
rg -tcpp "BUG"
echo "things you need to undo"
echo
rg -tcpp "UNDO"
echo "things you need to note"
echo
rg -tcpp "NOTE"