.model small
.stack 100h
.data
	InFileName 			db "input.txt"
	n 					dw ?
	errorMessage		db "impossible to multiply", 13, 10, '$'
	FileString 			db  100 dup (?)
			Matrix1 		dw 100 dup (?)		
			n1				dw ? 	
			m1				dw ?	
			Matrix2			dw 100 dup (?)
			n2				dw ?	
			m2				dw ?	
			MatrixResult	dw 100 dup (?)	
	StringResult 		db 200 dup (' ')
	endl 				db 13, 10
	handle 				dw ?
	OutFileName 		db "output.txt"


.code

proc MatrixMultiply


	lea di, Matrix1
	lea si, Matrix1
	mov ax, n1		
	mov dx, m1
	mul dx
	shl ax, 1		
	mov n, ax
	add di, ax		
	mov si, di
	sub di, ax		
	
	lea bx, MatrixResult
	mov dx, n1	

rowCycle:	
	mov cx, m2
	push dx
	
columnCycle:	
	xor ax, ax
	mov [bx], ax
	push cx
	mov cx, m1
	
multiply:	
	mov ax, word ptr [di]
	mov dx, word ptr [si]
	imul dx
	
	add [bx], ax		
	
	add di, 2			
	
	mov ax, 2
	mov dx, m2
	imul dx
	add si, ax			

loop multiply
	
	add bx, 2			


	sub si, ax
	mov ax, m2
	shl ax, 1
	mov cx, n2
	dec cx
	mul cx
	
	sub si, ax
	inc si
	inc si
	mov cx, m1
	mov ax, 2
	imul cx
	sub di, ax		

	pop cx
loop columnCycle
	
	mov ax, 2
	mov dx, m1
	imul dx
	add di, ax		

	lea si, Matrix1
	add si, n		
	
	pop dx
	dec dx
	JNZ rowCycle
	ret
endp MatrixMultiply



proc ReadFromFile
	push ax
	push bx
	push cx
	push dx
	
	xor dx, dx
    mov ah, 3Dh             
    xor al, al               
    lea dx, InFileName
    xor cx, cx              
    int 21h
	jc exitProgram
	
	mov handle, ax
	
	xor dx, dx
    mov bx, ax
    mov ah, 3Fh          
    lea dx, FileString
    mov cx, 95            
    int 21h
	cmp ax, cx
	jnz close
	
	xor bx, bx
	lea si, FileString		
    add si, ax			 
    
	inc si
	inc si
	
	mov byte ptr [si], '$'
		
	xor ax, ax
close:
	mov ah,3Eh			
    mov bx, handle
	int 21h                

	pop dx
	pop cx
	pop bx
	pop ax
	
	lea si, FileString	
	
	ret	
endp ReadFromFile


proc StringToMatrix
	xor ax, ax
	xor bp, bp
	xor dx, dx
	lea di, Matrix1	
	
begin:	
	xor ax, ax
	lodsb

	cmp al, '$'
	jz endOfString

	cmp al, '-'
	jne toNumber 

	inc bp
	jmp begin

toNumber:
	cmp al, '9'
	jg notNumber

	cmp al, '0'
	jb notNumber

	sub ax,'0'	
	shl dx,1	
	add ax, dx
	shl dx, 2
	add dx, ax	
	jmp begin

notNumber:
	cmp al, ' '
	jne newStringSymbol
	jmp number
newStringSymbol:
	inc si
	
number:
	mov ax, dx
	
	cmp bp, 1
	jne positiveNumber
	neg ax
	positiveNumber:
	mov [di], ax
	xor dx, dx
	inc di
	inc di
	xor bp, bp
	jmp begin
endOfString:
	ret
endp StringToMatrix


proc ReadDigit
	xor ax, ax
	lodsb		
	sub al, '0'	
	ret
endp ReadDigit


proc ReadN1M1N2M2
	call ReadDigit
	mov n1, ax
	inc si

	call ReadDigit
	mov m1, ax
	inc si

	call ReadDigit
	mov n2, ax
	inc si

	call ReadDigit
	mov m2, ax
	inc si
	inc si

	ret	
endp ReadN1M1N2M2




proc NumberToString
	add si, 6
	push cx
	push si
	xor bp, bp

	cmp ax, 0
	jg positive

	neg ax
	inc bp

positive:
	xor dx, dx
	mov cx, 10
	div cx
	mov byte ptr [si], '0'
	add [si], dl

	dec si

	cmp ax, 0
	jg positive

	cmp bp, 0
	je exitProc

	mov byte ptr [si], '-'
	xor bp, bp

exitProc:
	pop si
	inc si
	pop cx
	ret
endp NumberToString


proc MatrixToString
	
	lea si, StringResult
	lea di, MatrixResult
	mov cx, n1
allMatrix1:
	push cx

	mov cx, m2
outString:
	mov ax, [di]
	call NumberToString
	inc di
	inc di
	loop outString

	mov byte ptr [si], 13
	inc si
	mov byte ptr [si], 10
	inc si
	pop cx
	loop allMatrix1
	;mov byte ptr [si], '$'
	ret
endp MatrixToString


proc WriteInFile

    mov ah,3Ch             
    lea dx, OutFileName    
    xor cx,cx              
    int 21h                
	mov handle,ax         
 
    mov bx,ax             
    mov ah,40h            
    lea dx, StringResult  
    mov cx, 200         
    int 21h             

    mov ah, 3Eh         
    mov bx, handle      
    int 21h             	
	ret
endp WriteInFile

proc ErrorProc	
	
	mov ax, m1
	cmp ax, n2
	je exitProc1
	
	xor ax, ax
	xor dx, dx
	mov ah, 9
	lea dx, errorMessage
	int 21h
	mov ax, 1000
exitProc1:
	ret
endp ErrorProc


start:
	mov ax, @DATA 	
    mov ds, ax       
	mov es, ax
	
	call ReadFromFile
	call ReadN1M1N2M2
	
	call ErrorProc		
	cmp ax, 1000
	je exitProgram
	
	call StringToMatrix
	
	call MatrixMultiply
	
	call MatrixToString	
	call WriteInFile
	
exitProgram:		
	mov ax, 4c00h
	int 21h
  end start 