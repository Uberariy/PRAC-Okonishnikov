program FileTreePrint; 
{$mode TP}
const L=16; 
type str=packed array[1..L] of char; 
	tree=^chain; 
	chain=record 
		word: str;
		len, freq: integer; 
		left, right: tree; 
	end; 

var T1: tree; bookin, bookout: text; s: integer; n: 0..1;

procedure ReadFileandTreeBuild(var T: tree); 
var w: str;
	procedure TreeAdd(var T: tree; var w: str); 
	begin if T=nil then begin new(T); T^.word:=w; T^.freq:=1; T^.len:=s; 
			 T^.left:=nil; T^.right:=nil; T^.len:=s; 
		  end 
		  else if T^.word=w then begin T^.freq:=T^.freq+1; end 
			   else if T^.word<w then TreeAdd(T^.right,w) 
				    else TreeAdd(T^.left,w); 
	end;
	
	procedure FileReadWord(var w: str);
	var c: char; endread: boolean;
	begin for s:=1 to L do w[s]:=' '; s:=1; endread:=true; 
		while endread and (not eof(bookin)) do
		begin while endread and (not eoln(bookin)) do
			begin read(bookin, c);
				if (c=' ') or (c=',') or (c='.') then begin if s>1 then endread:=false; end
												 else begin w[s]:=c; s:=s+1; end;
				if eoln(bookin) and (s>1) then endread:=false
			end;
			if eoln(bookin) then readln(bookin);
		end;
	end; 
		
begin T:=nil; 
	assign(bookin, 'Animal_input.txt'); reset(bookin); 
	while not eof(bookin) do begin FileReadWord(w); TreeAdd(T,w); end; 
	close(bookin);
end; 

procedure TreePrint(T: tree); 
const shi='   ';
var i: integer; 
	procedure TreePrintIn(T: tree; i: integer); 
	var j: integer;
	begin if T<>nil then begin for j:=1 to i do write(shi); write(T^.word); writeln(T^.freq); 
			if (T^.right<>nil) or (T^.left<>nil) then 
			begin TreePrintIn(T^.left, i+1); TreePrintIn(T^.right, i+1) end 
		end 
		else begin for j:=1 to i do write(shi); writeln('-'); end
	end; 
	
begin i:=0; TreePrintIn(T,i) end; 

procedure FilePrint(T: tree);
const shi=' '; 
var i: integer;
	procedure FilePrintWord(T: tree; w: str; i,fr: integer);
	var j,d,k: integer; c: char;
	begin for j:=1 to i do write(bookout, shi);
		  for j:=1 to L do write(bookout, w[j]);
		  if fr>=0 then begin k:=1;
							  while fr>0 do begin
								d:=fr;
								while d>9 do d:=d div 10;
								c:=chr(d+ord('0'));
								write(bookout, c);
								k:=k+1;
								fr:=fr div 10;
							  end;
					    end;
		  writeln(bookout);
	end;

	procedure FilePrintIn(T: tree; i: integer); 
	begin if T<>nil then begin FilePrintWord(T,T^.word,i*3,T^.freq); if (T^.right<>nil) or (T^.left<>nil) then 
															   begin FilePrintIn(T^.left, i+1); FilePrintIn(T^.right, i+1) end 
						 end 
		  else FilePrintWord(T,'-',i*3,-1);
	end;

begin i:=0; 
	assign(bookout, 'Animal_output.txt'); rewrite(bookout);
	FilePrintIn(T,i)
end;

procedure PrintMin(T: tree); 
const shi='   ';
var i: integer; 
	procedure MinWord(T: tree; var i: integer);
	begin if T<>nil then 
		  begin if (T^.len<i) and (T^.len>=1) then i:=T^.len; MinWord(T^.left, i); MinWord(T^.right, i) end;
	end;
	
	procedure PrintIn(T: tree; var i: integer); 
	begin if T<>nil then begin if T^.len=i then write(T^.word,shi);
			if (T^.right<>nil) or (T^.left<>nil) then 
			begin PrintIn(T^.left, i); PrintIn(T^.right, i) end 
		end; 
	end;
begin writeln('Список слов минимальной длины:');
	  i:=L; MinWord(T, i); PrintIn(T, i);
end; 

procedure TreeDispose(T: tree);
begin if T<>nil then if (T^.right=nil) and (T^.left=nil) then dispose(T)
					 else begin TreeDispose(T^.right); TreeDispose(T^.left) end;
end;
	
begin 
	ReadFileandTreeBuild(T1); 
	writeln('Введите 0, если хотите, чтобы дерево было распечатано в интерфейсе и 1, для его записи в файл:');
	readln(n); 
	if n=0 then TreePrint(T1)
		   else FilePrint(T1);
	PrintMin(T1);
	TreeDispose(T1);
end.
