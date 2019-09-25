syntax on
colorscheme slate

set tabstop=4 
set shiftwidth=4 
set expandtab
set autoindent

set colorcolumn=80 
highlight ColorColumn ctermbg=darkgray

augroup project   
    autocmd!   
    autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen 
augroup END

set number

" This works on Windows, but is ineffective on Linux
set guifont=Lucida_Sans_Typewriter:h10
