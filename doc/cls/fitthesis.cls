% ----------------------------------------------------------------------------
% FIT thesis
% ----------------------------------------------------------------------------
% (C) 2007 Zdenek Vasicek
% vim: set syntax=tex encoding=latin2:
% ----------------------------------------------------------------------------
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{fitthesis}[2007/04/13 Sablona pro BP/DP]

%Vychozi hodnoty parametru
\let\ifczech\iftrue

%-----------------------------------------------------------------------------
%Parametry tridy
%-----------------------------------------------------------------------------
\DeclareOption{english}{\let\ifczech\iffalse}
\DeclareOption*{}
\ProcessOptions\relax
%Jako vychozi pouzijeme tridu REPORT
\ifx\pdfoutput\undefined % nejedeme pod pdftexem
   \LoadClass[a4paper,oneside,onecolumn,11pt]{report}
\else
   \LoadClass[pdftex,a4paper,oneside,onecolumn,11pt]{report}
\fi

%-----------------------------------------------------------------------------
%Balicky
%-----------------------------------------------------------------------------
\usepackage{amsmath}
\usepackage[czech,english]{babel}
\usepackage[latin2]{inputenc} %kodovani
\ifczech
  \main@language{czech}
\else
  \main@language{english}
\fi
\ifx\pdfoutput\undefined  % nejedeme pod pdftexem
  \usepackage{graphics}
  \usepackage{epsfig}
\else % je to pdftex !
  \usepackage[pdftex]{graphicx}
\fi

%-----------------------------------------------------------------------------
%Sazebni obrazec
%-----------------------------------------------------------------------------
\setlength{\hoffset}{-0.7cm}
\setlength{\voffset}{-1.5cm}
\setlength{\textheight}{23.0cm}
\setlength{\textwidth}{15.2cm}

%-----------------------------------------------------------------------------
%Texty
%-----------------------------------------------------------------------------
\def\@vut@CS{Vysoké uèení technické v~Brnì}
\def\@vut@EN{Brno University of Technology}
\def\@fit@CS{Fakulta informaèních technologií}
\def\@fit@EN{Faculty of Information Technology}
\def\@author@CS{Autoøi práce}
\def\@author@EN{Authors}
\def\@supervisor@CS{Vedoucí práce}
\def\@supervisor@EN{Supervisor}
\def\@dep@CS{Ústav}
\def\@dep@EN{Department of}
\def\@task@CS{Zadání}
\def\@licence@CS{Licenèní smlouva}
\def\@abstract@CS{Abstrakt}
\def\@abstract@EN{Abstract}
\def\@keywords@CS{Klíèová slova}
\def\@keywords@EN{Keywords}
\def\@cite@CS{Citace}
\def\@copyright@CS{Tato práce vznikla jako ¹kolní dílo na Vysokém uèení technickém v Brnì, Fakultì informaèních technologií. 
Práce je chránìna autorským zákonem a její u¾ití bez udìlení opráv\-nìní autorem je nezákonné, s výjimkou zákonem definovaných pøípadù.}
\def\@tdecl@CS{Prohlá¹ení}
\def\@tackn@CS{Podìkování}

\newif\ifnasel
\def\ifEqString#1#2#3#4{\def\testa{#1}\def\testb{#2}%
  \ifx\testa\testb \def#4{#3} \naseltrue \fi%
}

%Vyber typu projektu
\newcommand{\set@project}[1]{%
 \naselfalse
 \ifEqString{#1}{BP}{Týmový projekt do pøedmìtu PGR}{\@projecttype@CS}%
 \ifEqString{#1}{BP}{Team project to course PGR}{\@projecttype@EN}%
 \ifEqString{#1}{DP}{Diplomová práce}{\@projecttype@CS}%
 \ifEqString{#1}{DP}{Master's thesis}{\@projecttype@EN}%
 \ifnasel
 \else
   \typeout{Spatny typ projektu! Moznosti: BP, DP}\set@project{BP}
 \fi
}

%Vyber ustavu
\newcommand{\set@department}[1]{%
 \naselfalse
 \ifEqString{#1}{UIFS}{informaèních systémù}{\@department@CS}%
 \ifEqString{#1}{UIFS}{Information Systems}{\@department@EN}%
 \ifEqString{#1}{UPSY}{poèítaèových systémù}{\@department@CS}%
 \ifEqString{#1}{UPSY}{Computer Systems}{\@department@EN}%
 \ifEqString{#1}{UITS}{inteligentních systémù}{\@department@CS}%
 \ifEqString{#1}{UITS}{Intelligent Systems}{\@department@EN}%
 \ifEqString{#1}{UPGM}{poèítaèové grafiky a multimédií}{\@department@CS}%
 \ifEqString{#1}{UPGM}{Computer Graphics and Multimedia}{\@department@EN}%
 \ifnasel
 \else
   \typeout{Spatny ustav! Moznosti: UPSY, UIFS, UITS, UPGM}\set@department{UIFS}
 \fi
}

%Nazev ustavu (CS, EN)
\def\@departmentname@CS{\@dep@CS\ \@department@CS}
\def\@departmentname@EN{\@dep@EN\ \@department@EN}

%Nazev prace (v jazyce prace)
\def\@title{\ifczech\@title@CS\else\@title@EN\fi}

%Jmeno, prijmeni, tituly autora
\def\get@author{\ifx\undefined\@author@tp\else%
\@author@tp~\fi\MakeUppercase{\@author}\ifx\undefined\@author@ta\else,~\@author@ta\fi}
%Jmeno, prijmeni, tituly vedouciho
\def\get@supervisor{\ifx\undefined\@supervisor@tp\else%
\@supervisor@tp~\fi\MakeUppercase{\@supervisor}\ifx\undefined\@supervisor@ta\else,~\@supervisor@ta\fi}

%-----------------------------------------------------------------------------
% Projectinfo - parametrizace sablony
%-----------------------------------------------------------------------------
%Parametry 
\define@key{projinfo}{title.cs}{\global\def\@title@CS{#1}}
\define@key{projinfo}{title.en}{\global\def\@title@EN{#1}}
\define@key{projinfo}{department}{\set@department{#1}}
\define@key{projinfo}{project}{\set@project{#1}}
\define@key{projinfo}{year}{\def\@year{#1}}
\define@key{projinfo}{date}{\def\@date{#1}}
\define@key{projinfo}{author}{\def\@author{#1}}
\define@key{projinfo}{author.title.a}{\def\@author@ta{#1}}
\define@key{projinfo}{author.title.p}{\def\@author@tp{#1}}
\define@key{projinfo}{supervisor}{\def\@supervisor{#1}}
\define@key{projinfo}{supervisor.title.a}{\def\@supervisor@ta{#1}}
\define@key{projinfo}{supervisor.title.p}{\def\@supervisor@tp{#1}}
\define@key{projinfo}{keywords.cs}{\def\@tkeywords@CS{#1}}
\define@key{projinfo}{keywords.en}{\def\@tkeywords@EN{#1}}
\define@key{projinfo}{task.cs}{\def\@ttask@CS{#1}}
\define@key{projinfo}{licence.cs}{\def\@tlicence@CS{#1}}
\define@key{projinfo}{abstract.cs}{\def\@tabstract@CS{#1}}
\define@key{projinfo}{abstract.en}{\def\@tabstract@EN{#1}}
\define@key{projinfo}{declaration}{\def\@declaration{#1}}
\define@key{projinfo}{acknowledgment}{\def\@acknowledgment{#1}}

%Vychozi hodnoty
\setkeys{projinfo}{
  title.cs={Název práce (èesky)},title.en={Název práce(anglicky)}, 
  date=\today, project=BP, department=UIFS, declaration=, 
  abstract.cs=, abstract.en=, keywords.cs=, keywords.en=,
  author=Jmeno Prijmeni, supervisor=Jmeno Prijmeni
}%

\def\projectinfo{\setkeys{projinfo}}

%Prohlaseni
\newcommand{\declaration}[1]{\def\@declaration{#1}}

%Podekovani
\newcommand{\acknowledgment}[1]{\def\@acknowledgment{#1}}

%Zadani
\newcommand{\task}[2][cs]{%
 \ifEqString{#1}{cs}{#2}{\@ttask@CS}%
 \ifEqString{#1}{en}{#2}{\@ttask@EN}%
}

%Licencni smlouva
\newcommand{\licence}[2][cs]{%
 \ifEqString{#1}{cs}{#2}{\@tlicence@CS}%
 \ifEqString{#1}{en}{#2}{\@tlicence@EN}%
}

%Abstrakt
\renewcommand{\abstract}[2][cs]{%
 \ifEqString{#1}{cs}{#2}{\@tabstract@CS}%
 \ifEqString{#1}{en}{#2}{\@tabstract@EN}%
}

%Klicova slova
\newcommand{\keywords}[2][cs]{%
 \ifEqString{#1}{cs}{#2}{\@tkeywords@CS}%
 \ifEqString{#1}{en}{#2}{\@tkeywords@EN}%
}

\def\setfontsize#1{\fontsize{#1}{\f@baselineskip}\selectfont}
\newcommand{\mktxt}[2]{{\setfontsize{#1}\noindent#2}\newline}
\newcommand{\mkuctxt}[2]{{\setfontsize{#1}\noindent\MakeUppercase{#2}}\newline}

%-----------------------------------------------------------------------------
%Desky \makecover
%-----------------------------------------------------------------------------
\newcommand\makecover{\thispagestyle{empty}\null%
%===================================================
\fontfamily{phv}\selectfont%
%  
\mkuctxt{24}{\@vut@CS}
\mkuctxt{11}{\@vut@EN}

\vspace{12mm}

\mkuctxt{14}{\@fit@CS}
\mkuctxt{14}{\@departmentname@CS}

\vspace{1mm}

\mkuctxt{11}{\@fit@EN}
\mkuctxt{11}{\@departmentname@EN}

\vfill

\mkuctxt{16}{\@title} %Nazev prace v jazyce v jakem je napsana

\vfill

\mkuctxt{14}{\@projecttype@CS}
\mkuctxt{10}{\@projecttype@EN}
\vspace{5mm}

\mktxt{14}{\MakeUppercase{\@author@CS} \hfill \get@author}
\mkuctxt{10}{\@author@EN}

\vspace{10mm}

\mktxt{11}{BRNO \@year}

%===================================================
\newpage\fontfamily{\familydefault}\selectfont%
}

%-----------------------------------------------------------------------------
%Zadani
%-----------------------------------------------------------------------------
\newcommand\maketask{%
\thispagestyle{empty}\null%
\mktxt{18}{\@title}

\vspace{10mm}
\mktxt{16}{\@task@CS}\@ttask@CS
\newpage
}

%-----------------------------------------------------------------------------
%Licencni smlouva
%-----------------------------------------------------------------------------
\newcommand\makelicence{%
\thispagestyle{empty}\null%

\mktxt{16}{\@licence@CS}\@tlicence@CS
\newpage
}

%-----------------------------------------------------------------------------
%Abstrakt, klicovaslova \makeabstract
%-----------------------------------------------------------------------------
\newcommand\makeabstract{%
\thispagestyle{empty}\null%

\mktxt{16}{\@abstract@CS}\@tabstract@CS

\vfill

\mktxt{16}{\@keywords@CS}\@tkeywords@CS

\vfill

\mktxt{16}{\@abstract@EN}\@tabstract@EN

\vfill

\mktxt{16}{\@keywords@EN}\@tkeywords@EN

\vfill

\mktxt{16}{\@cite@CS}
\@author: \@title, \MakeLowercase{\@projecttype@CS}, Brno, FIT VUT v~Brnì, \@year
%===================================================
\newpage
}

%-----------------------------------------------------------------------------
%Prohlaseni, podekovani \makedeclaration
%-----------------------------------------------------------------------------
\newcommand\makedeclaration{
\thispagestyle{empty}%
\mktxt{18}{\@title}

\vspace{10mm}
\mktxt{16}{\@tdecl@CS}
\@declaration

  \begin{flushright}
      \makebox[4cm]{\dotfill} \\
      \@author\\
      \@date
  \end{flushright}

\ifx\undefined\@acknowledgment
\else
\vspace{20mm}
\mktxt{16}{\@tackn@CS}
\@acknowledgment
\fi

\vfill

\noindent\copyright\ \@author, \@year.\newline
{\em \@copyright@CS}
%===================================================
\newpage
}

%-----------------------------------------------------------------------------
%Uvodni stranky \maketitle
%-----------------------------------------------------------------------------
\renewcommand\maketitle{%
%\makecover%
\thispagestyle{empty}\null%
%=====================================================
\fontfamily{phv}\selectfont%
\newsavebox{\@obrlogo}\newsavebox{\@rtext}\newlength{\@vyskartext}\newlength{\@vyskaobr}%

\savebox{\@obrlogo}{\includegraphics[width=3.5cm,keepaspectratio]{cls/vut-zp2}}%logo VUT
\savebox{\@rtext}{\vbox{\mkuctxt{18}{\@vut@CS}}}%\mkuctxt{11}{\@vut@EN}}}%text VUT
\settoheight{\@vyskaobr}{\makebox{\usebox{\@obrlogo}}}% vyska loga
\settoheight{\@vyskartext}{\makebox{\usebox{\@rtext}}}% vyska textu
\addtolength{\@vyskaobr}{-\@vyskartext}%odecteme od vysky loga vysku textu
\noindent\usebox{\@obrlogo}\raisebox{\@vyskaobr}{\hspace{10pt}\usebox{\@rtext}}%zvedneme box s textem

\vspace{5mm}

\savebox{\@obrlogo}{\includegraphics[width=3.5cm,keepaspectratio]{cls/fit-zp2}}%logo FIT
\savebox{\@rtext}{%text FIT, USTAV
\vbox{\mkuctxt{14}{\@fit@CS}\mkuctxt{14}{\@departmentname@CS}

%\mkuctxt{11}{\@fit@EN}\mkuctxt{11}{\@departmentname@EN}
}}%
\settoheight{\@vyskaobr}{\makebox{\usebox{\@obrlogo}}}% vyska loga
\settoheight{\@vyskartext}{\makebox{\usebox{\@rtext}}}% vyska textu
\addtolength{\@vyskaobr}{-\@vyskartext}%odecteme od vysky loga vysku textu
\noindent\usebox{\@obrlogo}\raisebox{\@vyskaobr}{\hspace{10pt}\usebox{\@rtext}}%zvedneme box s textem

\vfill

\mkuctxt{16}{\@title@CS}
\mkuctxt{10}{\@projecttype@CS}

\vfill

%\mkuctxt{14}{\@projecttype@CS}
%\mkuctxt{10}{\@projecttype@EN}


\mktxt{14}{\MakeUppercase{\@author@CS} \hfill}% \get@author}
\mktxt{10}{David Baøina}
\mktxt{10}{Kamil Dudka}
\mktxt{10}{Jakub Filák}
\mktxt{10}{Luká¹ Hefka}


%\mktxt{14}{\MakeUppercase{\@supervisor@CS} \hfill \get@supervisor}
%\mkuctxt{10}{\@supervisor@EN}

\vspace{10mm}

\mktxt{11}{BRNO \@year}

\newpage\fontfamily{\familydefault}\selectfont%
%=================================================================================
\maketask
% \makelicence
% \clearpage
% \setcounter{tocdepth}{2}\setcounter{page}{1}
% 
% \newpage\fontfamily{\familydefault}\selectfont%
% %=================================================================================
% \makeabstract\makedeclaration
\clearpage
\setcounter{tocdepth}{2}\setcounter{page}{1}
}

%-----------------------------------------------------------------------------
\def\uv#1{\iflanguage{english}{``#1''}%
                              {\quotedblbase #1\textquotedblleft}}%
\catcode`\"=\active
\def"{\bgroup
    \iflanguage{english}%
        {``\def"{''\egroup}}%
        {\quotedblbase\def"{\textquotedblleft\egroup}}}

\endinput
%%
%% End of file `fitprj.cls'.
