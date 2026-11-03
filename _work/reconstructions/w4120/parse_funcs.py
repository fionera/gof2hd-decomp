import sys,re

def strip_comments_strings(s):
    # replace comments and strings with spaces, preserving newlines
    out=[]; i=0; n=len(s)
    while i<n:
        c=s[i]
        if c=='/' and i+1<n and s[i+1]=='/':
            j=s.find('\n',i)
            if j<0: j=n
            out.append(' '*(j-i)); i=j
        elif c=='/' and i+1<n and s[i+1]=='*':
            j=s.find('*/',i+2)
            if j<0: j=n-2
            seg=s[i:j+2]
            out.append(''.join('\n' if ch=='\n' else ' ' for ch in seg)); i=j+2
        elif c in '"\'':
            q=c; j=i+1
            while j<n:
                if s[j]=='\\': j+=2; continue
                if s[j]==q: j+=1; break
                j+=1
            seg=s[i:j]
            out.append(''.join('\n' if ch=='\n' else ' ' for ch in seg)); i=j
        else:
            out.append(c); i+=1
    return ''.join(out)

def funcs(path):
    s=open(path).read(); t=strip_comments_strings(s)
    n=len(t); i=0; depth=0; start=0; res=[]
    # track preprocessor lines: skip them entirely (they may contain braces)
    while i<n:
        c=t[i]
        if c=='#' and (i==0 or t[i-1]=='\n'):
            j=t.find('\n',i)
            if j<0: j=n
            if depth==0: start=j+1
            i=j; continue
        if c=='{':
            if depth==0:
                head=t[start:i]
                # find last ';' or '}' to start of declaration
                k=max(head.rfind(';'),head.rfind('}'))
                decl=head[k+1:].strip()
                # classify
                first=decl.split(None,1)[0] if decl.split() else ''
                is_type = re.match(r'^(class|struct|namespace|union|enum|extern)\b',decl) is not None
                if not is_type and '(' in decl and not decl.startswith('typedef'):
                    # find matching brace
                    d=0; j=i
                    while j<n:
                        if t[j]=='{': d+=1
                        elif t[j]=='}':
                            d-=1
                            if d==0: break
                        j+=1
                    res.append((start,j+1,decl))
                    i=j+1; start=i; continue
            depth+=1; i+=1; continue
        if c=='}':
            depth-=1; i+=1
            if depth==0: start=i
            continue
        if c==';' and depth==0:
            start=i+1
        i+=1
    return res

if __name__=='__main__':
    for p in sys.argv[1:]:
        fs=funcs(p)
        print(p,len(fs))
        for a,b,d in fs:
            print('  ',d.split('\n')[-1][:100])
