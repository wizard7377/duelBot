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