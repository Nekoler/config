"nvim
set number
set numberwidth=1
set tabstop=4
set cursorline
nmap w :w<CR>
nmap q :q<CR>
colorscheme industry
"plugin
call plug#begin('~/.config/nvim/plug')
Plug 'Yggdroot/indentLine'
Plug 'vim-airline/vim-airline'
Plug 'tomasiser/vim-code-dark'
Plug 'neoclide/coc.nvim',{'branch':'release'}
call plug#end()
let g:airline_theme='codedark'
let g:airline_powerline_fonts=1
let g:indentLine_char_list=['┆','│']
autocmd BufRead,BufNewFile *.json let g:indentLine_conceallevel=0
autocmd BufRead,BufNewFile * if expand('%:e')!='typ'|set nowrap|endif
autocmd BufRead,BufNewFile * if expand('%:e')=='c' |set shiftwidth=2|else|set shiftwidth=4|endif
"coc.nvim
hi CocFloating ctermfg=white ctermbg=darkblue
inoremap <expr> <CR> coc#pum#visible()?coc#_select_confirm():'<CR>'
