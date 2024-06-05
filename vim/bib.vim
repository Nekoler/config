function! coc#source#bib#citer()
        let filepath=expand('%:p:h').'/Lib/bibliography.bib'
        if filereadable(filepath)
                let bib_citers=[]
                let bib_citer=readfile(filepath)
                let bib_citer=matchstrlist(bib_citer,'@[a-z]\+{\w\+,')
                for i in bib_citer
			call add(bib_citers,substitute(i['text'][:-2],'@[a-z]\+{','',''))
                endfor
        endif
        return bib_citers
endfunction

function! coc#source#bib#init()
    return {'triggerCharacters': ['@'],'triggerOnly': 0,'filetypes': ['markdown']}
endfunction

function! coc#source#bib#complete(option, cb)
	call a:cb(coc#source#bib#citer())
endfunction
