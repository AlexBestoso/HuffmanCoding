let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
argglobal
if bufexists("~/Documents/GithubRepositories/HuffmanCoding/c++/HuffmanCoding.class.h") | buffer ~/Documents/GithubRepositories/HuffmanCoding/c++/HuffmanCoding.class.h | else | edit ~/Documents/GithubRepositories/HuffmanCoding/c++/HuffmanCoding.class.h | endif
setlocal keymap=
setlocal noarabic
setlocal noautoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=s1:/*,mb:*,ex:*/,://,b:#,:%,:XCOMM,n:>,fb:-
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
setlocal nocursorline
setlocal cursorlineopt=both
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal noexpandtab
if &filetype != 'cpp'
setlocal filetype=cpp
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
setlocal nolist
setlocal listchars=
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal nomodeline
setlocal modifiable
setlocal nrformats=bin,octal,hex
setlocal number
setlocal numberwidth=4
setlocal omnifunc=
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=8
setlocal noshortname
setlocal showbreak=
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=0
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal spelloptions=
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'cpp'
setlocal syntax=cpp
endif
setlocal tabstop=8
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=0
setlocal thesaurus=
setlocal thesaurusfunc=
setlocal noundofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal virtualedit=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
15,51fold
54,57fold
59,62fold
64,67fold
69,76fold
78,81fold
83,87fold
89,92fold
94,100fold
102,105fold
107,110fold
54,110fold
113,117fold
119,123fold
125,130fold
132,140fold
142,150fold
152,157fold
159,164fold
166,171fold
173,179fold
181,186fold
188,193fold
113,193fold
196,208fold
210,222fold
224,236fold
238,254fold
256,272fold
274,286fold
288,300fold
302,314fold
316,328fold
330,342fold
344,356fold
196,356fold
359,402fold
404,440fold
442,480fold
482,521fold
523,581fold
583,642fold
644,687fold
689,724fold
359,725fold
733,776fold
778,794fold
796,816fold
818,833fold
838,862fold
864,1022fold
1024,1038fold
728,1038fold
1042,1068fold
1074,1295fold
1297,1350fold
1041,1350fold
1353,1366fold
1379,1449fold
1451,1525fold
1527,1537fold
1539,1610fold
1353,1610fold
1613,1682fold
1684,1757fold
1759,1839fold
1613,1839fold
1842,1918fold
1921,1956fold
1842,1956fold
1968,2002fold
2004,2038fold
2040,2136fold
2138,2191fold
1960,2191fold
2194,2197fold
2201,2202fold
2205,2231fold
2234,2247fold
2250,2299fold
2302,2345fold
2349,2383fold
2349,2401fold
2406,2567fold
let &fdl = &fdl
let s:l = 2569 - ((376 * winheight(0) + 14) / 29)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 2569
normal! 02|
let &g:so = s:so_save | let &g:siso = s:siso_save
doautoall SessionLoadPost
" vim: set ft=vim :
