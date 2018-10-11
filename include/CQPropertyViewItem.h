#ifndef CQPropertyViewItem_H
#define CQPropertyViewItem_H

#include <CQUtil.h>
#include <QObject>
#include <QPointer>
#include <QStyleOptionViewItem>
#include <QString>
#include <QVariant>
#include <vector>
#include <cassert>

class CQPropertyViewDelegate;
class CQPropertyViewEditorFactory;
class QPainter;
class QWidget;

class CQPropertyViewItem : public QObject {
  Q_OBJECT

 public:
  typedef std::vector<CQPropertyViewItem *> Children;

 public:
  CQPropertyViewItem(CQPropertyViewItem *parent, QObject *object, const QString &name);

 ~CQPropertyViewItem();

  bool isValid() const { return id_ == 0xFEEDBEEF; }

  CQPropertyViewItem *parent() const { return parent_; }

  QObject *object() const { return object_; }

  const Children &children() const { return children_; }

  CQPropertyViewItem *child(int i) const {
    assert(i >= 0 && i < numChildren());

    return children_[i];
  }

  int numChildren() const { return children_.size(); }

  const QString &name() const { return name_; }
  void setName(const QString &s) { name_ = s; }

  const QString &alias() const { return alias_; }
  void setAlias(const QString &v) { alias_ = v; }

  void addChild(CQPropertyViewItem *row);

  void removeChild(CQPropertyViewItem *row);

  bool isEditable() const { return editable_; }
  void setEditable(bool b) { editable_ = b; }

  bool isInside() const { return inside_; }
  void setInside(bool b) { inside_ = b; }

  const QStringList &values() const { return values_; }
  void setValues(const QStringList &v) { values_ = v; }

  QString aliasName() const;

  QString path(const QString &sep="/", bool alias=false, CQPropertyViewItem *root=nullptr) const;

  //! set editor
  void setEditorFactory(CQPropertyViewEditorFactory *editor) { editor_ = editor; }

  //! handle click
  bool click();

  //! create editor widget
  QWidget *createEditor(QWidget *parent);

  //! set widget property from variant
  void setEditorData(const QVariant &value);

  //! get editor widget text
  QVariant getEditorData() const;

  //! is writable
  bool isWritable() const;

  //! get/set data
  QVariant data() const;
  bool setData(const QVariant &value);

  //! get tip
  QString tip() const;

  //! paint item
  bool paint(const CQPropertyViewDelegate *delegate, QPainter *painter,
             const QStyleOptionViewItem &option, const QModelIndex &index);

 private:
  QWidget *createDefaultEdit(QWidget *parent, const QString &valueStr);

  QString getDefaultValue() const;

 signals:
  void valueChanged(QObject *obj, const QString &name);

 private slots:
  //! called when the editor widget is changed
  void updateValue();

  bool enumIndToString(const CQUtil::PropInfo &propInfo, int ind, QString &str) const;
  bool enumStringToInd(const CQUtil::PropInfo &propInfo, const QString &str, int &ind) const;

 private:
  uint                         id_       { 0xFEEDBEEF };
  CQPropertyViewItem*          parent_   { nullptr };
  QPointer<QObject>            object_;
  QString                      name_;
  QString                      alias_;
  Children                     children_;
  bool                         editable_ { false };
  bool                         inside_   { false };
  QWidget*                     widget_   { nullptr };
  CQPropertyViewEditorFactory *editor_   { nullptr }; //! editor interface
  QStringList                  values_;
};

#endif
