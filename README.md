[![Build Status](https://travis-ci.org/misterdjules/node-asm-ref.svg?branch=master)](https://travis-ci.org/misterdjules/node-asm-ref)

node-asm-ref is a [Node.js](http://nodejs.org/) binding for the [libx86-ref
library](https://github.com/misterdjules/libx86-ref).

It allows to query a database about assembly language instructions to ask
questions such as:
* "What are the instructions starting with the string "ADD"?"
* "What are the register flags affected by the CALL instruction?".
* "How does the RET instruction works in details?".

It is useful when creating documentation tools for assembly language, like
searchable online documentation or to integrate a reference documentation
into a program like a code editor or an IDE.