import fitz
import json

doc = fitz.open('rev/thesis_rev17_20260324.pdf')
comments = []
for page_num in range(len(doc)):
    page = doc[page_num]
    annots = page.annots()
    if annots:
        for annot in annots:
            info = annot.info
            content = info.get('content', '').strip()
            title = info.get('title', '').strip()
            subject = info.get('subject', '').strip()
            atype = annot.type[1] if annot.type else ''
            # Get the text the annotation refers to
            rect = annot.rect
            highlighted_text = ''
            if atype in ['Highlight', 'Underline', 'StrikeOut', 'Squiggly']:
                highlighted_text = page.get_text('text', clip=rect).strip()
            comments.append({
                'page': page_num + 1,
                'type': atype,
                'author': title,
                'content': content,
                'highlighted': highlighted_text,
                'subject': subject
            })

print(f"Total comments found: {len(comments)}")
print("=" * 80)
for i, c in enumerate(comments, 1):
    print(f"[{i}] Page {c['page']} | Type: {c['type']} | Author: {c['author']}")
    if c['highlighted']:
        hl = c['highlighted'][:200]
        print(f"    HIGHLIGHTED: {hl}")
    if c['content']:
        ct = c['content'][:500]
        print(f"    COMMENT: {ct}")
    print("-" * 60)

# Also save to JSON for later use
with open('rev17_comments.json', 'w', encoding='utf-8') as f:
    json.dump(comments, f, ensure_ascii=False, indent=2)
print(f"\nSaved to rev17_comments.json")
