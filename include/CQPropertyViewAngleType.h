#ifndef CQPropertyViewAngleEditor_H
#define CQPropertyViewAngleEditor_H

#include <CQPropertyViewType.h>

/*!
 * \brief type for CAngle
 */
class CQPropertyViewAngleType : public CQPropertyViewType {
 public:
  CQPropertyViewAngleType();

  CQPropertyViewEditorFactory *getEditor() const override;

  bool setEditorData(CQPropertyViewItem *item, const QVariant &value) override;

  void draw(CQPropertyViewItem *item, const CQPropertyViewDelegate *delegate,
            QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index,
            const QVariant &value, const ItemState &itemState) override;

  QString tip(const QVariant &value) const override;
};

//------

#include <CQPropertyViewEditor.h>

/*!
 * \brief editor factory for CAngle
 */
class CQPropertyViewAngleEditor : public CQPropertyViewEditorFactory {
 public:
  CQPropertyViewAngleEditor();

  QWidget *createEdit(QWidget *parent) override;

  void connect(QWidget *w, QObject *obj, const char *method) override;

  QVariant getValue(QWidget *w) override;

  void setValue(QWidget *w, const QVariant &var) override;
};

#endif
