#ifndef CQPropertyViewRealType_H
#define CQPropertyViewRealType_H

#include <CQPropertyViewType.h>

/*!
 * \brief type for double
 */
class CQPropertyViewRealType : public CQPropertyViewType {
 public:
  CQPropertyViewRealType();

  CQPropertyViewEditorFactory *getEditor() const override;

  bool setEditorData(CQPropertyViewItem *item, const QVariant &value) override;

  void draw(CQPropertyViewItem *item, const CQPropertyViewDelegate *delegate,
            QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index,
            const QVariant &value, const ItemState &itemState) override;
};

//------

#include <CQPropertyViewEditor.h>

/*!
 * \brief editor factory for double
 */
class CQPropertyViewRealEditor : public CQPropertyViewEditorFactory {
 public:
  enum class Type {
    DoubleSpin,
    RealSlider,
    ComboSlider
  };

 public:
  CQPropertyViewRealEditor(double min=-1E50, double max=1E50, double step=1.0);

  const Type &type() const { return type_; }
  void setType(const Type &v) { type_ = v; }

  QWidget *createEdit(QWidget *parent) override;

  void connect(QWidget *w, QObject *obj, const char *method) override;

  QVariant getValue(QWidget *w) override;

  void setValue(QWidget *w, const QVariant &var) override;

 private:
  Type   type_ { Type::DoubleSpin };
  double min_  { -1E50 };
  double max_  { 1E50 };
  double step_ { 1.0 };
};

#endif
